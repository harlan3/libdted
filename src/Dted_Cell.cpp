//*****************************************************************************
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero, Oscar Kramer
//
// DESCRIPTION:
//   Contains implementation of class Dted_Cell. This class derives from
//   ElevHandler. It is responsible for loading an individual DTED cell
//   from disk. This elevation files are memory mapped.
//
//*****************************************************************************

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Dted_Cell.h"
#include "Dted_Vol.h"
#include "Dted_Hdr.h"
#include "Dted_Uhl.h"
#include "Dted_Dsi.h"
#include "Dted_Acc.h"
#include "Dted_Record.h"
#include "Endian.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Dted_Cell::Dted_Cell(const string& dted_file)
   :
      theFileStr(),
      theNumLonLines(0),
      theNumLatPoints(0),
      theDtedRecordSizeInBytes(0),
      theEdition(),
      theProductLevel(),
      theCompilationDate(),
      theOffsetToFirstDataRecord(0),
      theLatSpacing(0.0),
      theLonSpacing(0.0),
      theSwCornerPost(),
      byteSwap(false)
{
    Endian endianObj;

    theFilename = dted_file;

    if (!open()) {

        cerr << "FATAL: Could not open file:  "
                << dted_file.c_str() << "\nReturning..." << std::endl;
        cerr << "Returning with error..." << std::endl;

        return;
    }

    // DTED is stored in big endian.
    byteSwap =
            endianObj.getSystemEndianType() == ARCH_LITTLE_ENDIAN ?
                    true : false;

    // Attempt to parse.
    Dted_Vol vol(theFileStr, 0);
    Dted_Hdr hdr(theFileStr, vol.stopOffset());
    Dted_Uhl uhl(theFileStr, hdr.stopOffset());
    Dted_Dsi dsi(theFileStr, uhl.stopOffset());
    Dted_Acc acc(theFileStr, dsi.stopOffset());

    theNumLonLines = uhl.numLonLines();
    theNumLatPoints = uhl.numLatPoints();
    theLatSpacing = uhl.latInterval();
    theLonSpacing = uhl.lonInterval();

    theDtedRecordSizeInBytes = (theNumLatPoints * POST_SIZE)
            + Dted_Record::DATA_LENGTH;

    theEdition = dsi.edition();
    theProductLevel = dsi.productLevel();
    theCompilationDate = dsi.compilationDate();

    theOffsetToFirstDataRecord = acc.stopOffset();

    theSwCornerPost.lat = uhl.latOrigin();
    theSwCornerPost.lon = uhl.lonOrigin();

    theNullHeightValue = 0.0;

    dtedPostMemPtr = NULL;
    debug = false;
}

bool Dted_Cell::covers(Geo_Location targetLoc) {
    bool latCovered = false;
    bool lonCovered = false;

    double deltaLat = targetLoc.lat - theSwCornerPost.lat;
    double deltaLon = targetLoc.lon - theSwCornerPost.lon;

    if ((deltaLat >= 0) && (deltaLat < 1.0))
        latCovered = true;
    else
        latCovered = false;

    if ((deltaLon >= 0) && (deltaLon < 1.0))
        lonCovered = true;
    else
        lonCovered = false;

    if (latCovered && lonCovered)
        return true;
    else
        return false;
}

void Dted_Cell::loadCellFromDisk() {
    boost::mutex::scoped_lock lock(sharedMutex);

    int offset;

    dtedPostMemPtr = (unsigned char *) malloc(
            (theNumLonLines * theNumLatPoints) * POST_SIZE);

    memset(dtedPostMemPtr, 0, (theNumLonLines * theNumLatPoints) * POST_SIZE);

    theFileStr.seekg(theOffsetToFirstDataRecord, ios::beg);

    for (int i = 0; i < theNumLonLines; i++) {
        theFileStr.seekg(DATA_RECORD_OFFSET_TO_POST, ios::cur); // 8 bytes

        offset = i * (theNumLatPoints * POST_SIZE);

        theFileStr.read((char*) &dtedPostMemPtr[offset],
                (theNumLatPoints * POST_SIZE));

        theFileStr.seekg(DATA_RECORD_CHECKSUM_SIZE, ios::cur);  // 4 bytes
    }
}

Dted_Cell::~Dted_Cell() {
    close();

    if (dtedPostMemPtr != NULL)
        free(dtedPostMemPtr);
}

bool Dted_Cell::open() {
    boost::mutex::scoped_lock lock(sharedMutex);

    if (theFileStr.is_open()) {
        return true;
    }

    theFileStr.open(theFilename.c_str(), ios::in | ios::binary);

    return theFileStr.is_open();
}

void Dted_Cell::close() {
    boost::mutex::scoped_lock lock(sharedMutex);

    if (theFileStr.is_open()) {
        theFileStr.close();
    }
}

void Dted_Cell::setBilinearInterpActive(bool newState) {
    bilinearInterpActive = newState;
}

double Dted_Cell::getHeightAboveMSL(const Geo_Location& gpt) {
    // Establish the grid indexes
    double xi = fabs(gpt.lon - theSwCornerPost.lon) * (theNumLonLines - 1);
    double yi = fabs(gpt.lat - theSwCornerPost.lat) * (theNumLatPoints - 1);

    // Check for right edge.
    int x0 = static_cast<int>(xi);
    int y0 = static_cast<int>(yi);

    if ((gpt.lon < theSwCornerPost.lon) || (gpt.lat < theSwCornerPost.lat)
            || (x0 >= theNumLonLines - 1) || (y0 >= theNumLatPoints - 1)) {
        return theNullHeightValue;
    }

    //***
    // Grab the four points from the dted cell needed.  These are big endian,
    // signed magnitude shorts so they must be interpreted accordingly.
    //***

    int offset = (int) ((x0 * theNumLatPoints * POST_SIZE) + (y0 * POST_SIZE));

    unsigned short us;
    signed short ss;
    double p00;   // Post 1 (Bottom left, where X(lon) & Y(lat))
    double p01;   // Post 2 (Top left)
    double p10;   // Post 3 (Bottom right)
    double p11;   // Post 4 (Top right)

    // Get the first post.

    memcpy(&us, &dtedPostMemPtr[offset], sizeof(unsigned short));
    ss = convertSignedMagnitude(us);
    p00 = ss;

    if (!bilinearInterpActive)
        return p00;

    // Get the second post (i.e. below post #1 due to longitude)
    offset += POST_SIZE;
    memcpy(&us, &dtedPostMemPtr[offset], sizeof(unsigned short));
    ss = convertSignedMagnitude(us);
    p01 = ss;

    // Get the third post (i.e. on the right of post #1, therefore have to
    // offset back 1 post size to lign-up on the right of post #1).
    offset += (theNumLonLines * POST_SIZE) - POST_SIZE;

    // The following 'if' should never be true but need the check
    // to avoid accessing out-of-bound memory
    if (yi > (double) (theNumLatPoints - 1)) {
        yi = (int) yi;
    }

    memcpy(&us, &dtedPostMemPtr[offset], sizeof(unsigned short));
    ss = convertSignedMagnitude(us);
    p10 = ss;

    // Get the fourth post (i.e. below post #3).
    offset += POST_SIZE;

    memcpy(&us, &dtedPostMemPtr[offset], sizeof(unsigned short));
    ss = convertSignedMagnitude(us);
    p11 = ss;

    double interPolatedElev = bilinearInterpolate(xi, yi, p00, p01, p10, p11);

    return interPolatedElev;

}

double Dted_Cell::getHeightAboveMSLFromDisk(const Geo_Location& gpt) {
    boost::mutex::scoped_lock lock(sharedMutex);

    double xi = fabs(gpt.lon - theSwCornerPost.lon) * (theNumLonLines - 1);
    double yi = fabs(gpt.lat - theSwCornerPost.lat) * (theNumLatPoints - 1);

    // Check for right edge.
    int x0 = static_cast<int>(xi);
    int y0 = static_cast<int>(yi);

    // Do some error checking.
    if ((xi < 0.0) || (yi < 0.0) || (x0 >= theNumLonLines - 1)
            || (y0 >= theNumLatPoints - 1))  // Last line
            {
        return theNullHeightValue;
    }

    // Grab the four points from the dted cell needed.  These are big endian,
    // signed magnitude shorts so they must be interpreted accordingly.

    int offset = theOffsetToFirstDataRecord + (x0 * theDtedRecordSizeInBytes)
            + DATA_RECORD_OFFSET_TO_POST + (y0 * POST_SIZE);

    // Put the file pointer at the start of the first elevation post.
    theFileStr.seekg(offset, ios::beg);

    unsigned short us;
    signed short ss;
    double p00;   // Post 1 (Bottom left, where X(lon) & Y(lat))
    double p01;   // Post 2 (Top left)
    double p10;   // Post 3 (Bottom right)
    double p11;   // Post 4 (Top right)

    // Get the Post 1.
    theFileStr.read((char*) &us, POST_SIZE);
    ss = convertSignedMagnitude(us);
    p00 = ss;

    if (!bilinearInterpActive)
        return p00;

    // Get Post 2.
    theFileStr.read((char*) &us, POST_SIZE);
    ss = convertSignedMagnitude(us);
    p01 = ss;

    // Get Post 3.
    offset += theDtedRecordSizeInBytes - POST_SIZE;
    theFileStr.seekg(offset, ios::beg);

    if (!theFileStr.eof()) {
        // Get the third post.
        theFileStr.read((char*) &us, POST_SIZE);
        ss = convertSignedMagnitude(us);
        p10 = ss;
    } else {
        p10 = p00;
    }

    if (!theFileStr.eof()) {
        // Get Post 4.
        theFileStr.read((char*) &us, POST_SIZE);
        ss = convertSignedMagnitude(us);
        p11 = ss;
    } else {
        p11 = p01;
    }

    return bilinearInterpolate(xi, yi, p00, p01, p10, p11);
}

double Dted_Cell::bilinearInterpolate(double xi, double yi, double p00,
        double p01, double p10, double p11) {
    // p00   // Post 1 (Bottom left, where X(lon) & Y(lat))
    // p01   // Post 2 (Top left)
    // p10   // Post 3 (Bottom right)
    // p11   // Post 4 (Top right)

    // Check for right edge.
    int x0 = static_cast<int>(xi);
    int y0 = static_cast<int>(yi);

    // Perform bilinear interpolation:
    double wx1 = xi - x0;
    double wy1 = yi - y0;
    double wx0 = 1.0 - wx1;
    double wy0 = 1.0 - wy1;

    double w00 = wx0 * wy0;
    double w01 = wx0 * wy1;
    double w10 = wx1 * wy0;
    double w11 = wx1 * wy1;

    // Test for null posts and set the corresponding weights to 0:
    if (p00 == NULL_POST)
        w00 = 0.0;
    if (p01 == NULL_POST)
        w01 = 0.0;
    if (p10 == NULL_POST)
        w10 = 0.0;
    if (p11 == NULL_POST)
        w11 = 0.0;

#if 0 /* Serious debug only... */
    cout << "\np00:  " << p00
    << "\np01:  " << p01
    << "\np10:  " << p10
    << "\np11:  " << p11
    << "\nw00:  " << w00
    << "\nw01:  " << w01
    << "\nw10:  " << w10
    << "\nw11:  " << w11
    << endl;
#endif

    double sum_weights = w00 + w01 + w10 + w11;

    if (sum_weights)
        return (p00 * w00 + p01 * w01 + p10 * w10 + p11 * w11) / sum_weights;

    return NULL_POST;
}

double Dted_Cell::getPostValue(const Voxel& gridPt) {

    // Do some error checking.
    if (gridPt.x < 0.0 || gridPt.y < 0.0 || gridPt.x > (theNumLonLines - 1)
            || gridPt.y > (theNumLatPoints - 1)) {
        cerr << "WARNING : No intersection..." << std::endl;
        return theNullHeightValue;
    }

    int offset = (int) (gridPt.x * (theNumLatPoints * POST_SIZE)
            + gridPt.y * POST_SIZE);

    unsigned short us;
    memcpy(&us, &dtedPostMemPtr[offset], sizeof(unsigned short));

    // Get the post.
    return double(convertSignedMagnitude(us));
}

double Dted_Cell::getPostValueFromDisk(const Voxel& gridPt) {
    boost::mutex::scoped_lock lock(sharedMutex);

    // Do some error checking.
    if (gridPt.x < 0.0 || gridPt.y < 0.0 || gridPt.x > (theNumLonLines - 1)
            || gridPt.y > (theNumLatPoints - 1)) {
        cerr << "WARNING " << ": No intersection..." << std::endl;
        return theNullHeightValue;
    }

    int offset = (int) (theOffsetToFirstDataRecord
            + gridPt.x * theDtedRecordSizeInBytes + gridPt.y * POST_SIZE
            + DATA_RECORD_OFFSET_TO_POST);

    // Put the file pointer at the start of the first elevation post.
    theFileStr.seekg(offset, ios::beg);

    unsigned short us;

    // Get the post.
    theFileStr.read((char*) &us, POST_SIZE);

    return double(convertSignedMagnitude(us));
}

void Dted_Cell::gatherStatistics() {
    boost::mutex::scoped_lock lock(sharedMutex);

    // Check to see if there is a statistics file already.  If so; do a lookup
    // for the min and max values.
    string stats_file = theFilename;

    theMinHeightAboveMSL = 32767;
    theMaxHeightAboveMSL = -32767;

    // Put the file pointer at the start of the first elevation post.
    theFileStr.seekg(theOffsetToFirstDataRecord, ios::beg);

    // Loop through all records and scan for lowest min and highest max.
    // Each record contains a row of latitude points for a given longitude.
    // There are eight bytes in front of the post and four checksum bytes at
    // the end so ignore them.
    for (int i = 0; i < theNumLonLines; ++i)  // longitude direction
            {
        theFileStr.seekg(DATA_RECORD_OFFSET_TO_POST, ios::cur);

        for (int j = 0; j < theNumLatPoints; ++j) // latitude direction
                {
            unsigned short us;
            signed short ss;
            theFileStr.read((char*) &us, POST_SIZE);

            ss = convertSignedMagnitude(us);
            if (ss < theMinHeightAboveMSL && ss != NULL_POST) {
                theMinHeightAboveMSL = ss;
            }
            if (ss > theMaxHeightAboveMSL) {
                theMaxHeightAboveMSL = ss;
            }
        }

        theFileStr.seekg(DATA_RECORD_CHECKSUM_SIZE, ios::cur);
    }

    if (debug) {
        cout << "Stats for file = " << stats_file << endl;

        cout << "Dted_Cell::gatherStatistics:" << "\ntheMinHeightAboveMSL:  "
                << theMinHeightAboveMSL << "\ntheMaxHeightAboveMSL:  "
                << theMaxHeightAboveMSL << std::endl;
    }
}

Cell_Size Dted_Cell::getSizeOfElevCell() const {
    Cell_Size returnVal;
    returnVal.lonLines = theNumLonLines;
    returnVal.latPoints = theNumLatPoints;

    return returnVal;
}

string Dted_Cell::edition() const {
    return theEdition;
}

string Dted_Cell::productLevel() const {
    return theProductLevel;
}

string Dted_Cell::compilationDate() const {
    return theCompilationDate;
}

float Dted_Cell::minHeightAboveMSL() const {
    return theMinHeightAboveMSL;
}

float Dted_Cell::maxHeightAboveMSL() const {
    return theMaxHeightAboveMSL;
}

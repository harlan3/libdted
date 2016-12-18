//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Data Record Description
//               of a DTED file.
//
// Notes:  Each elevation is a true value referenced to meas sea level
//         (MSL) datum recorded to the nearest meter.  The horizontal
//         position is referenced to precise longitude-latitiude
//         locations in terms of the current World Geodetic System
//         (WGS84) determined for each file by reference to the origin
//         at the southwest corner.  The elevations are evenly spaced
//         in latitude and longitude at the interval designated in the
//         User Header Label (UHL) in South to North profile sequence.
//
//********************************************************************

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "Dted_Record.h"
#include "Dted_Common.h"
#include "Endian.h"

static const unsigned short DATA_NULL_VALUE = 0xffff; // -32767
static const unsigned short DATA_MIN_VALUE = 0xfffe; // -32766
static const unsigned short DATA_MAX_VALUE = 0x7fff; // +32767
static const unsigned short DATA_VALUE_MASK = 0x7fff; // 0111 1111 1111 1111
static const unsigned short DATA_SIGN_MASK = 0x8000; // 1000 0000 0000 0000
static const unsigned short DATA_RECOGNITION_SENTINEL = 0xAA;  // 170

//***
// Offsets from start of data record (one record per longitude line):
//***
static const int BLOCK_COUNT_OFFSET = 2;
static const int LON_INDEX_OFFSET = 4;
static const int LAT_INDEX_OFFSET = 6;
static const int ELEV_DATA_OFFSET = 8;
static const int RECORD_HDR_LENGTH = 12;
static const int BYTES_PER_POINT = 2;

Dted_Record::Dted_Record(istream& in, int offset, int num_points) :
        theRecSen("170"),
        theDataBlockCount(0),
        theLonCount(0),
        theLatCount(0),
        theCheckSum(0),
        thePoints(new int[num_points]),
        thePointsData(new unsigned short[num_points]),
        theComputedCheckSum(0),
        theNumPoints(num_points),
        theStartOffset(offset),
        theStopOffset(offset + RECORD_HDR_LENGTH + (num_points * BYTES_PER_POINT)) {
    // Verify we are at a cell record by checking the Recognition Sentinel.
    unsigned char buf[1];
    in.seekg(theStartOffset, ios::beg);
    in.read((char*) buf, 1);

#if 0
    printf("\nBuf: %02X", (int)buf[0]);  // Display in HEX
#endif

    if (!(buf[0] = DATA_RECOGNITION_SENTINEL)) {
        cerr
                << "FATAL Dted_Record::Dted_Record: reading DTED's data record at: "
                << theStartOffset << std::endl;
        return;
    }

    // Valid data record, so let's process on.
    parse(in);

    //***
    // Note:  The validateCheckSum method works; however, our in-house
    //        dted has bad stored check sums even though the post are good.
    //        So this in temporarily shut off.
    //***
    // Verify Check Sum for uncorrupted elevation data.
    if (validateCheckSum(in)) {
        //***
        // Note:  The validateCheckSum method works; however, our in-house
        //        dted has bad stored check sums even though the posts are good.
        //        So this in temporarily shut off.
        //***
        cerr << "FATAL Dted_Record::Dted_Record:"
                << "\nInvalid checksum in data record at: " << theStartOffset
                << "\nParsed Check Sum = " << theCheckSum
                << "\nComputed Check Sum = " << theComputedCheckSum
                << "\nDTED Level 1 Elevation File is considered corrupted."
                << std::endl;
        return;
    }
}

Dted_Record::~Dted_Record() {
    if (thePoints) {
        delete[] thePoints;
        thePoints = NULL;
    }
    if (thePointsData) {
        delete[] thePointsData;
        thePointsData = NULL;
    }
}

void Dted_Record::parse(istream& in) {
    Endian endian;

    // DTED is stored in big endian byte order so swap the bytes if needed.
    bool swap_bytes =
            endian.getSystemEndianType() == ARCH_LITTLE_ENDIAN ? true : false;

    signed short s;

    // parse data block count
    in.seekg(theStartOffset + BLOCK_COUNT_OFFSET, ios::beg);
    in.read((char*) &s, 2);
    theDataBlockCount = (swap_bytes ? ((s << 8) | (s >> 8)) : s);

    // parse lon count
    in.seekg(theStartOffset + LON_INDEX_OFFSET, ios::beg);
    in.read((char*) &s, 2);
    theLonCount = (swap_bytes ? ((s << 8) | (s >> 8)) : s);

    // parse lat count
    in.seekg(theStartOffset + LAT_INDEX_OFFSET, ios::beg);
    in.read((char*) &s, 2);
    theLatCount = (swap_bytes ? ((s << 8) | (s >> 8)) : s);

    // Parse all elevation points.
    in.seekg(theStartOffset + ELEV_DATA_OFFSET, ios::beg);
    int i = 0;
    for (i = 0; i < theNumPoints; i++) {
        in.read((char*) &s, 2);
        s = (swap_bytes ? ((s << 8) | (s >> 8)) : s);
        if (s & DATA_SIGN_MASK)
            s = (s & DATA_VALUE_MASK) * -1;
        int value = convert(s);

        // Check to make sure value is within valid limits for a value.
        if (value == DBL_NAN) {
            cerr
                    << "WARNING Dted_Record::parse: Null Value found at point index: "
                    << i << ", continuing..." << std::endl;
        }

        thePoints[i] = value;
        thePointsData[i] = s;
    }
}

int Dted_Record::validateCheckSum(istream& in) {
    Endian endian;

    // DTED is stored in big endian byte order so swap the bytes if needed.
    bool swap_bytes =
            endian.getSystemEndianType() == ARCH_LITTLE_ENDIAN ? true : false;

    // Compute the check sum.
    in.seekg(theStartOffset, ios::beg);
    theComputedCheckSum = 0;
    int bytesToRead = (theNumPoints * 2) + ELEV_DATA_OFFSET;
    int i = 0;

    for (i = 0; i < bytesToRead; i++) {
        unsigned char c;
        in.read((char*) &c, 1);
        theComputedCheckSum += static_cast<unsigned int>(c);
    }

    // Read the stored check sum and swap the byte if needed.
    in.read((char*) &theCheckSum, 4);
    if (swap_bytes) {
        Endian swapper;
        swapper.swap(theCheckSum);
    }

    // Compare computed and parsed checksums.
    if (theCheckSum != theComputedCheckSum)
        return TRUE;
    else
        return FALSE;
}

int Dted_Record::convert(signed short val) {
    if (val == -32767) // Null value.
        return static_cast<int>(INT_NAN);
    else
        return static_cast<int>(val);
}

string Dted_Record::recoginitionSentinel() const {
    return theRecSen;
}

int Dted_Record::dataBlockCount() const {
    return theDataBlockCount;
}

int Dted_Record::lonCount() const {
    return theLonCount;
}

int Dted_Record::latCount() const {
    return theLatCount;
}

unsigned int Dted_Record::checkSum() const {
    return theCheckSum;
}

unsigned int Dted_Record::computedCheckSum() const {
    return theComputedCheckSum;
}

int Dted_Record::numPoints() const {
    return theNumPoints;
}

int Dted_Record::getPoint(int i) const {
    return thePoints[i];
}

unsigned short Dted_Record::getPointData(int i) const {
    return thePointsData[i];
}

int* Dted_Record::points() const {
    return thePoints;
}

unsigned short* Dted_Record::pointsData() const {
    return thePointsData;
}

int Dted_Record::startOffset() const {
    return theStartOffset;
}

int Dted_Record::stopOffset() const {
    return theStopOffset;
}

ostream& operator<<(ostream& os, const Dted_Record& rec) {
    os << "\nDTED Record:" << "\n-------------------------------"
            << "\nRecoginition Sentinel: " << rec.theRecSen
            << "\nData Block Count:      " << rec.theDataBlockCount
            << "\nLon Count:             " << rec.theLonCount
            << "\nLat Count:             " << rec.theLatCount
            << "\nNum Points:            " << rec.theNumPoints
            << "\nParsed Check Sum:      " << rec.theCheckSum
            << "\nComputed Check Sum     " << rec.theComputedCheckSum
            << "\nStart Offset:          " << rec.theStartOffset
            << "\nStop Offset:           " << rec.theStopOffset << endl;
    int i = 0;

    for (i = 0; i < rec.theNumPoints; i++) {
        os << "\nPoint[" << i << "]: " << rec.thePoints[i];
    }

    os << endl;

    return os;
}

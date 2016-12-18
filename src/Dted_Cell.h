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

#ifndef Dted_Cell_HEADER
#define Dted_Cell_HEADER

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <fstream>

#include <string>
#include "Dted_Common.h"

using namespace std;

class Dted_Cell {
public:

    //! Dted cell constructor
    Dted_Cell(const string& dted_file);

    virtual ~Dted_Cell();

    enum {
        DATA_RECORD_OFFSET_TO_POST = 8,     // bytes
        DATA_RECORD_CHECKSUM_SIZE = 4,     // bytes
        POST_SIZE = 2,     // bytes
        NULL_POST = -32767 // Fixed by DTED specification.
    };

    //! Retrieve Height above MSL from memory.
    double getHeightAboveMSL(const Geo_Location& gpt);

    //! Retrieve Height above MSL from disk.
    double getHeightAboveMSLFromDisk(const Geo_Location& gpt);

    //! Returns the number of post in the cell.
    Cell_Size getSizeOfElevCell() const;

    //! Load dted cell from disk into memory.
    void loadCellFromDisk();

    //! Enable/Disable bilinear intepolation
    void setBilinearInterpActive(bool newState);

    //! Returns an elevation post from a dted cell on disk.
    double getPostValueFromDisk(const Voxel& gridPt);

    //! Returns an elevation post from a dted cell in memory.
    double getPostValue(const Voxel& gridPt);

    //! Return the Dted edition.
    string edition() const;

    //! Return the Dted product level.
    string productLevel() const;

    //! Return the Dted compilation date.
    string compilationDate() const;

    //! Open a stream to the dted cell.
    //! @return Returns true on success, false on error.
    bool open();

    //! Closes the stream to the file.
    void close();

    //! Return the minimum height above MSL for cell
    float minHeightAboveMSL() const;

    //! Return the maximum height above MSL for cell
    float maxHeightAboveMSL() const;

    //! Calculate statistics on dted cell
    void gatherStatistics();

    //! Returns true if the cell covers the target location.
    bool covers(Geo_Location targetLoc);

    //! Perform bilinear interpolation on four points.
    //! p00: Post 1 (Bottom left, where X(lon) & Y(lat))
    //! p01: Post 2 (Top left)
    //! p10: Post 3 (Bottom right)
    //! p11: Post 4 (Top right)
    double bilinearInterpolate(double xi, double yi, double p00, double p01,
            double p10, double p11);

private:

    // Disallow operator= and copy constrution...
    const Dted_Cell& operator=(const Dted_Cell& rhs) {
        return rhs;
    }
    Dted_Cell(const Dted_Cell&) {
    }

    //! Convert unsigned short to signed magnitude.
    inline signed short convertSignedMagnitude(unsigned short& s) {
        s = (byteSwap ? ((s << 8) | (s >> 8)) : s);

        if (s & 0x8000)
            return (static_cast<signed short>(s & 0x7fff) * -1);

        return static_cast<signed short>(s);
    }

    mutable ifstream theFileStr;
    int theNumLonLines;  // east-west dir
    int theNumLatPoints; // north-south

    int theDtedRecordSizeInBytes;
    string theEdition;
    string theProductLevel;
    string theCompilationDate;
    int theOffsetToFirstDataRecord;
    double theLatSpacing;   // degrees
    double theLonSpacing;   // degrees
    Geo_Location theSwCornerPost;

    float theNullHeightValue;
    float theMinHeightAboveMSL;
    float theMaxHeightAboveMSL;

    bool bilinearInterpActive;
    bool byteSwap;

    string theFilename;

    mutable boost::mutex sharedMutex;

    unsigned char* dtedPostMemPtr;

    bool debug;
};

#endif

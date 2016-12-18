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

#ifndef Dted_Record_H
#define Dted_Record_H
#include <iostream>

#include "string.h"
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

class Dted_Record {
public:

    //! Dted record constructor.
    Dted_Record(std::istream& in, int offest, int num_points);
    // NOTE:  When using this class be sure that as you cycle through
    //        all the points within a record, that you verify they are
    //        sequential.  According to the DTED Specification
    //        (MIL-PRF-89020A) issued in 19 April 1996, page 22 all
    //        records must be sequential.  If not, file may be corrupt.
    //
    //        To verify this, you can do the following:
    //
    //        int count = 0;
    //        for(int i = 0; i < num_lon_lines; i++)
    //          rec[i] = new Dted_Record(theFileDesc, offset,
    //                       num_lat_points);
    //          if(count != (rec[i]->dataBlockCount() + 1))
    //             ERROR -- Records are not sequential
    //          count = rec[i]->dataBlockCount();

    ~Dted_Record();

    enum {
        DATA_LENGTH = 12,
        DATA_BLOCK_COUNT = 2,
        DATA_LON_COUNT = 4,
        DATA_LAT_COUNT = 6,
        DATA_ELEV_START = 8,
        DATA_BYTES_PER_POINT = 2,
    };

    //! The Recoginition Sentinel signifies if the Data Record exists.
    string recoginitionSentinel() const;

    //! Return the datablock count.
    int dataBlockCount() const;

    //! Return the lonCount.
    int lonCount() const;

    //! Return the latCount.
    int latCount() const;

    //! Return the checksum.
    unsigned int checkSum() const;

    //! Return the computedCheckSum.
    unsigned int computedCheckSum() const;

    //! Return the numPoints.
    int numPoints() const;

    //! Access point data from the elevation data
    int getPoint(int i) const;

    //! Access point data from the elevation data
    unsigned short getPointData(int i) const;

    //! Access point data from the elevation data
    int* points() const;

    //! Access point data from the elevation data
    unsigned short* pointsData() const;

    //! Return the startOffset.
    int startOffset() const;

    //! Return the stopOffset.
    int stopOffset() const;

    friend ostream& operator<<(ostream& os, const Dted_Record& rec);
    void parse(std::istream& in);

private:
    // prevent use
    Dted_Record(const Dted_Record& source);

    int theFile;

    /*!
     * 252 (8 bit)
     */
    string theRecSen;

    /*!
     *  Sequential count of the block within the file.
     */
    int theDataBlockCount;

    /*!
     *  Count of the meridian.
     *  True longitude = longitude count x data interval + origin
     *  (Offset from the SW corner longitude)
     */
    int theLonCount;

    /*!
     *  Count of the parallel.
     *  True latitude = latitude count x data interval + origin
     *  (Offset from the SW corner latitude)
     */
    int theLatCount;

    /*!
     *  Algebraic addition of contents of block.
     *  The checksum is computed algebraically using integer arithmetic by
     *  summing all header and elevation bytes contained int the record as
     *  8-bit values.  Each byte is considered an unsigned, 8-bit value for
     *  checksum calculation.
     */
    unsigned int theCheckSum;

    /*!
     *  All the elevation points in a Data Record as ints.
     *  (ie. all the latitudal points in a longitudinal line)
     */
    int* thePoints;

    /*!
     *  All the elevation points in a Data Record as unsigned shorts.
     *  (ie. all the latitudal points in a longitudinal line)
     */
    unsigned short* thePointsData;

    /*!
     *  Our computed check sum.  This should match the checksum
     *  at the end of the Data Record.
     */
    unsigned int theComputedCheckSum;

    /*!
     *  The number of points in a longitudinal line.
     */
    int theNumPoints;

    int theStartOffset;
    int theStopOffset;

    /*!
     *  Converts signed short to an interger.  Returns NAN if val is
     *  a null value (-32767).
     */
    int convert(signed short val);

    /*!
     *  Compute the check sum for the Data Record and compare against
     *  the parsed check sum from the data record. This must be correct
     *  to be a valid data record. If not, there is a chance of a
     *  corrupted elevation cell.
     */
    int validateCheckSum(std::istream& in);
};

#endif

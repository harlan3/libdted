//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Ken Melero
// 
// Description:  This class provides access to the User Header Label
//               (UHL) of a DTED file.
//
//********************************************************************

#ifndef Dted_Uhl_H
#define Dted_Uhl_H
#include <iostream>
#include <string>

class Dted_Uhl {
public:

    //! Dted Uhl constructor
    Dted_Uhl(const string& dted_file, int offset);

    //! Dted Uhl constructor
    Dted_Uhl(std::istream& in, int offset);

    enum {
        UHL_LENGTH = 80,
        UHL_LON_ORIGIN = 5,
        UHL_LAT_ORIGIN = 13,
        UHL_LON_INTERVAL = 21,
        UHL_LAT_INTERVAL = 25,
        UHL_ABSOLUTE_LE = 29,
        UHL_SECURITY_CODE = 33,
        UHL_REFERENCE_NUM = 33,
        UHL_NUM_LON_LINES = 48,
        UHL_NUM_LAT_LINES = 52,
        UHL_MULTIPLE_ACC = 56,
        UHL_RESERVED = 57,
        FIELD1_SIZE = 3,
        FIELD2_SIZE = 1,
        FIELD3_SIZE = 8,
        FIELD4_SIZE = 8,
        FIELD5_SIZE = 4,
        FIELD6_SIZE = 4,
        FIELD7_SIZE = 4,
        FIELD8_SIZE = 3,
        FIELD9_SIZE = 12,
        FIELD10_SIZE = 4,
        FIELD11_SIZE = 4,
        FIELD12_SIZE = 1,
        FIELD13_SIZE = 24
    };

    //! The Recoginition Sentinel signifies if the UHL record exists.
    string recoginitionSentinel() const;

    //! Return the lonOrigin.
    double lonOrigin() const;

    //! Return the latOrigin.
    double latOrigin() const;

    //! Return the lonInterval.
    double lonInterval() const;

    //! Return the latInterval.
    double latInterval() const;

    //! Return the absolute Linerar Error
    double absoluteLE() const;

    //! Return the securityCode.
    string securityCode() const;

    //! Return the numLonLines.
    int numLonLines() const;

    //! Return the numLatPoints.
    int numLatPoints() const;

    //! Return the mulitpleAccuracy.
    int mulitpleAccuracy() const;

    //! Return the startOffset.
    int startOffset() const;

    //! Return the stopOffset.
    int stopOffset() const;

    friend std::ostream& operator<<(std::ostream& os, const Dted_Uhl& uhl);

    //! Parse the dted uhl.
    void parse(std::istream& in);

private:

    // Do not allow...
    Dted_Uhl(const Dted_Uhl& source);
    const Dted_Uhl& operator=(const Dted_Uhl& rhs);

    double degreesFromString(const char* str) const;
    double spacingFromString(const char* str) const;

    char theRecSen[FIELD1_SIZE + 1];
    char theField2[FIELD2_SIZE + 1];
    char theLonOrigin[FIELD3_SIZE + 1];
    char theLatOrigin[FIELD4_SIZE + 1];
    char theLonInterval[FIELD5_SIZE + 1];
    char theLatInterval[FIELD6_SIZE + 1];
    char theAbsoluteLE[FIELD7_SIZE + 1];
    char theSecurityCode[FIELD8_SIZE + 1];
    char theField9[FIELD9_SIZE + 1];
    char theNumLonLines[FIELD10_SIZE + 1];
    char theNumLatPoints[FIELD11_SIZE + 1];
    char theMultipleAccuracy[FIELD12_SIZE + 1];

    int theStartOffset;
    int theStopOffset;
};

#endif

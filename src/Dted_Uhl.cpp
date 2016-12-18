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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

#include "Dted_Uhl.h"

Dted_Uhl::Dted_Uhl(const string& dted_file, int offset) :
        theRecSen(),
        theField2(),
        theLonOrigin(),
        theLatOrigin(),
        theLonInterval(),
        theLatInterval(),
        theAbsoluteLE(),
        theSecurityCode(),
        theNumLonLines(),
        theNumLatPoints(),
        theMultipleAccuracy(),
        theStartOffset(offset),
        theStopOffset(0) {

    ifstream in(dted_file.c_str());
    if (!in) {
        cerr << "FATAL Dted_Uhl::Dted_Uhl: Error opening the DTED file: "
                << dted_file << std::endl;

        return;
    }

    parse(in);

    in.close();
}

Dted_Uhl::Dted_Uhl(istream& in, int offset) :
        theRecSen(), theLonOrigin(), theLatOrigin(), theLonInterval(), theLatInterval(), theAbsoluteLE(), theSecurityCode(), theNumLonLines(), theNumLatPoints(), theMultipleAccuracy(), theStartOffset(
                offset), theStopOffset(0) {
    parse(in);
}

void Dted_Uhl::parse(istream& in) {
    // Seek to the start of the record.
    in.seekg(theStartOffset, ios::beg);

    // Parse theRecSen
    in.read(theRecSen, FIELD1_SIZE);
    theRecSen[FIELD1_SIZE] = '\0';

    if (!(strncmp(theRecSen, "UHL", 3) == 0)) {
        // Not a user header label.
        cerr << "Error encountered in Dted_Uhl::parse(istream& in)" << endl;
        return;
    }

    // Parse Field 2
    in.read(theField2, FIELD2_SIZE);
    theField2[FIELD2_SIZE] = '\0';

    // Parse theLonOrigin
    in.read(theLonOrigin, FIELD3_SIZE);
    theLonOrigin[FIELD3_SIZE] = '\0';

    // Parse theLatOrigin
    in.read(theLatOrigin, FIELD4_SIZE);
    theLatOrigin[FIELD4_SIZE] = '\0';

    // Parse theLonInterval
    in.read(theLonInterval, FIELD5_SIZE);
    theLonInterval[FIELD5_SIZE] = '\0';

    // Parse theLatInterval
    in.read(theLatInterval, FIELD6_SIZE);
    theLatInterval[FIELD6_SIZE] = '\0';

    // Parse theAbsoluteLE
    in.read(theAbsoluteLE, FIELD7_SIZE);
    theAbsoluteLE[FIELD7_SIZE] = '\0';

    // Parse theSecurityCode
    in.read(theSecurityCode, FIELD8_SIZE);
    theSecurityCode[FIELD8_SIZE] = '\0';

    // Parse Field 9
    in.read(theField9, FIELD9_SIZE);
    theField9[FIELD9_SIZE] = '\0';

    // Parse theNumLonLines
    in.read(theNumLonLines, FIELD10_SIZE);
    theNumLonLines[FIELD10_SIZE] = '\0';

    // Parse theNumLatPoints
    in.read(theNumLatPoints, FIELD11_SIZE);
    theNumLatPoints[FIELD11_SIZE] = '\0';

    // Parse theMultipleAccuracy
    in.read(theMultipleAccuracy, FIELD12_SIZE);
    theMultipleAccuracy[FIELD12_SIZE] = '\0';

    // Field 13 not parsed as it's unused.

    // Set the stop offset.
    theStopOffset = theStartOffset + UHL_LENGTH;
}

string Dted_Uhl::recoginitionSentinel() const {
    return theRecSen;
}

double Dted_Uhl::lonOrigin() const {
    return degreesFromString(theLonOrigin);
}

double Dted_Uhl::latOrigin() const {
    return degreesFromString(theLatOrigin);
}

double Dted_Uhl::lonInterval() const {
    return spacingFromString(theLonInterval);
}

double Dted_Uhl::latInterval() const {
    return spacingFromString(theLatInterval);
}

double Dted_Uhl::degreesFromString(const char* str) const {
    // Parse the string:  DDDMMMSSH
    if (!str) {
        cerr << "FATAL Dted_Uhl::degreesFromString: "
                << "Null pointer passed to method!" << std::endl;
        return 0.0;
    }

    if (strlen(str) < 8) {
        cerr << "FATAL Dted_Uhl::decimalDegreesFromString:"
                << "String not big enough!" << std::endl;
        return 0.0;
    }

    double d = ((str[0] - '0') * 100 + (str[1] - '0') * 10 + (str[2] - '0')
            + (str[3] - '0') / 6.0 + (str[4] - '0') / 60.0
            + (str[5] - '0') / 360.0 + (str[6] - '0') / 3600.0);

    if ((str[7] == 'S') || (str[7] == 's') || (str[7] == 'W')
            || (str[7] == 'w')) {
        d *= -1.0;
    }

    return d;
}

double Dted_Uhl::spacingFromString(const char* str) const {
    // Parse the string: SSSS (tenths of a second)
    if (!str) {
        cerr << "FATAL Dted_Uhl::decimalDegreesFromString: "
                << "Null pointer passed to method!" << std::endl;
        return 0.0;
    }

    return atof(str) / 36000.0;  // return 10ths of second as decimal degrees.
}

double Dted_Uhl::absoluteLE() const {
    return (theAbsoluteLE ? atoi(theAbsoluteLE) : 0);
}

string Dted_Uhl::securityCode() const {
    return theSecurityCode;
}

int Dted_Uhl::numLonLines() const {
    return (theNumLonLines ? atoi(theNumLonLines) : 0);
}

int Dted_Uhl::numLatPoints() const {
    return (theNumLatPoints ? atoi(theNumLatPoints) : 0);
}

int Dted_Uhl::mulitpleAccuracy() const {
    return (theMultipleAccuracy ? atoi(theMultipleAccuracy) : 0);
}

int Dted_Uhl::startOffset() const {
    return theStartOffset;
}

int Dted_Uhl::stopOffset() const {
    return theStopOffset;
}

ostream& operator<<(ostream& os, const Dted_Uhl& uhl) {
    os << "\nDTED Header (UHL):" << "\n-------------------------------"
            << "\nRecoginition Sentinel: " << uhl.theRecSen
            << "\nLon Origin:            " << uhl.theLonOrigin
            << "\nLat Origin:            " << uhl.theLatOrigin
            << "\nLon Interval:          " << uhl.theLonInterval
            << "\nLat Interval:          " << uhl.theLatInterval
            << "\nAbsolute LE:           " << uhl.theAbsoluteLE
            << "\nSecurity Code:         " << uhl.theSecurityCode
            << "\nNumber of Lon Lines:   " << uhl.theNumLonLines
            << "\nNumber of Lat Lines:   " << uhl.theNumLatPoints
            << "\nMultiple Accuracy:     " << uhl.theMultipleAccuracy
            << "\nStart Offset:          " << uhl.theStartOffset
            << "\nStop Offset:           " << uhl.theStopOffset << endl;

    return os;
}

Dted_Uhl::Dted_Uhl(const Dted_Uhl& source) {
}

const Dted_Uhl& Dted_Uhl::operator=(const Dted_Uhl& rhs) {
    return rhs;
}


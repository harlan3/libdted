//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class provides access to the Volume Header Label
//               (VOL) of a DTED file.
//
//********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

#include "Dted_Vol.h"

Dted_Vol::Dted_Vol(const string& dted_file, int offset) :
        theRecSen(),
        theField2(),
        theReelNumber(),
        theField4(),
        theField5(),
        theAccountNumber(),
        theField7(),
        theField8(),
        theStartOffset(offset),
        theStopOffset(0) {

    // Open the dted file for reading.
    ifstream in(dted_file.c_str());
    if (!in) {
        cerr << "FATAL Dted_Vol::Dted_Vol" << "\nUnable to open the DTED file: "
                << dted_file << std::endl;
        return;
    }

    parse(in);

    in.close();
}

Dted_Vol::Dted_Vol(istream& in, int offset) :
        theRecSen(),
        theReelNumber(),
        theAccountNumber(),
        theStartOffset(offset),
        theStopOffset(0) {
    parse(in);
}

void Dted_Vol::parse(istream& in) {
    // Seek to the start of the record.
    in.seekg(theStartOffset, ios::beg);

    // Parse theRecSen
    in.read(theRecSen, FIELD1_SIZE);
    theRecSen[FIELD1_SIZE] = '\0';

    if (!(strncmp(theRecSen, "VOL", 3) == 0)) {
        // Not a volume header label.
        return;
    }

    // Parse Field 2
    in.read(theField2, FIELD2_SIZE);
    theField2[FIELD2_SIZE] = '\0';

    // Parse theReelNumber
    in.read(theReelNumber, FIELD3_SIZE);
    theReelNumber[FIELD3_SIZE] = '\0';

    // Parse Field 4
    in.read(theField4, FIELD4_SIZE);
    theField4[FIELD4_SIZE] = '\0';

    // Parse Field 5
    in.read(theField5, FIELD5_SIZE);
    theField5[FIELD5_SIZE] = '\0';

    // Parse theAccountNumber
    in.read(theAccountNumber, FIELD6_SIZE);
    theAccountNumber[FIELD6_SIZE] = '\0';

    // Parse Field 7
    in.read(theField7, FIELD7_SIZE);
    theField7[FIELD7_SIZE] = '\0';

    // Parse Field 8
    in.read(theField8, FIELD8_SIZE);
    theField8[FIELD8_SIZE] = '\0';

    // Set the stop offset.
    theStopOffset = theStartOffset + VOL_LENGTH;
}

string Dted_Vol::getRecoginitionSentinel() const {
    return string(theRecSen);
}

string Dted_Vol::getReelNumber() const {
    return string(theReelNumber);
}

string Dted_Vol::getAccountNumber() const {
    return string(theAccountNumber);
}

int Dted_Vol::startOffset() const {
    return theStartOffset;
}

int Dted_Vol::stopOffset() const {
    return theStopOffset;
}

ostream& operator<<(ostream& os, const Dted_Vol& vol) {
    os << "\nDTED Header (VOL):" << "\n-------------------------------"
            << "\nRecoginition Sentinel: " << vol.theRecSen
            << "\nField 2:               " << vol.theField2
            << "\ntheReelNumber:         " << vol.theReelNumber
            << "\nField 4:               " << vol.theField4
            << "\nField 5:               " << vol.theField5
            << "\ntheAccountNumber:      " << vol.theAccountNumber
            << "\nField 7:               " << vol.theField7
            << "\nField 8:               " << vol.theField8 << endl;

    return os;
}

Dted_Vol::Dted_Vol(const Dted_Vol& source) {
}

const Dted_Vol& Dted_Vol::operator=(const Dted_Vol& rhs) {
    return rhs;
}


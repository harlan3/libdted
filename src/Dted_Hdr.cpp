//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Ken Melero
// 
// Description:  This class gives access to the File Header Label
//               (HDR) of a DTED file.
//
//********************************************************************

#include <stdio.h>
#include <string.h>
#include <fstream>
using namespace std;

#include "Dted_Hdr.h"

Dted_Hdr::Dted_Hdr(const string& dted_file, int offset) :
        theRecSen(), theField2(), theFilename(), theField4(), theField5(), theField6(), theVersion(), theCreationDate(), theField9(), theField10(), theField11(), theField12(), theField13(), theField14(), theStartOffset(
                offset), theStopOffset(0) {

    ifstream in(dted_file.c_str());

    // open the dted file for reading
    if (!in) {
        cerr << "FATAL Dted_Hdr::Dted_Hdr: Error opening the DTED file: "
                << dted_file << std::endl;
        return;
    }

    // continue parsing all the record fields.
    parse(in);

    in.close();
}

Dted_Hdr::Dted_Hdr(istream& in, int offset) :
        theRecSen(), theField2(), theFilename(), theField4(), theField5(), theField6(), theVersion(), theCreationDate(), theField9(), theField10(), theField11(), theField12(), theField13(), theField14(), theStartOffset(
                offset), theStopOffset(0) {
    parse(in);
}

void Dted_Hdr::parse(istream& in) {
    // Seek to the start of the record.
    in.seekg(theStartOffset, ios::beg);

    // Parse theRecSen
    in.read(theRecSen, FIELD1_SIZE);
    theRecSen[FIELD1_SIZE] = '\0';

    if (!(strncmp(theRecSen, "HDR", 3) == 0)) {
        //Does not contain a volume header.
        return;
    }

    // Parse Field 2
    in.read(theField2, FIELD2_SIZE);
    theField2[FIELD2_SIZE] = '\0';

    // Parse theFilename
    in.read(theFilename, FIELD3_SIZE);
    theFilename[FIELD3_SIZE] = '\0';

    // Parse Field 4
    in.read(theField4, FIELD4_SIZE);
    theField4[FIELD4_SIZE] = '\0';

    // Parse Field 5
    in.read(theField5, FIELD5_SIZE);
    theField5[FIELD5_SIZE] = '\0';

    // Parse Field 6
    in.read(theField6, FIELD6_SIZE);
    theField6[FIELD6_SIZE] = '\0';

    // Parse Field 7
    in.read(theVersion, FIELD7_SIZE);
    theVersion[FIELD7_SIZE] = '\0';

    // Parse theCreationDate
    in.read(theCreationDate, FIELD8_SIZE);
    theCreationDate[FIELD8_SIZE] = '\0';

    // Parse Field 9
    in.read(theField9, FIELD9_SIZE);
    theField9[FIELD9_SIZE] = '\0';

    // Parse Field 10
    in.read(theField10, FIELD10_SIZE);
    theField10[FIELD10_SIZE] = '\0';

    // Parse Field 11
    in.read(theField11, FIELD11_SIZE);
    theField11[FIELD11_SIZE] = '\0';

    // Parse Field 12
    in.read(theField12, FIELD12_SIZE);
    theField12[FIELD12_SIZE] = '\0';

    // Parse Field 13
    in.read(theField13, FIELD13_SIZE);
    theField13[FIELD13_SIZE] = '\0';

    // Parse Field 14
    in.read(theField14, FIELD14_SIZE);
    theField14[FIELD14_SIZE] = '\0';

    // Set the stop offset.
    theStopOffset = theStartOffset + HDR_LENGTH;
}

ostream& operator<<(ostream& os, const Dted_Hdr& hdr) {
    os << "\nDTED Header (VOL):" << "\n-------------------------------"
            << "\ntheRecSen:             " << hdr.theRecSen
            << "\nField 2:               " << hdr.theField2
            << "\ntheFilename:           " << hdr.theFilename
            << "\nField 4:               " << hdr.theField4
            << "\nField 5:               " << hdr.theField5
            << "\nField 6:               " << hdr.theField6
            << "\nVersion:               " << hdr.theVersion
            << "\ntheCreationDate:       " << hdr.theCreationDate
            << "\nField 9:               " << hdr.theField9
            << "\nField 10:              " << hdr.theField10
            << "\nField 11:              " << hdr.theField11
            << "\nField 12:              " << hdr.theField12
            << "\nField 13:              " << hdr.theField13
            << "\nField 14:              " << hdr.theField14 << endl;

    return os;
}

string Dted_Hdr::recoginitionSentinel() const {
    return theRecSen;
}

string Dted_Hdr::fileName() const {
    return theFilename;
}

string Dted_Hdr::version() const {
    return theVersion;
}

string Dted_Hdr::creationDate() const {
    return theCreationDate;
}

int Dted_Hdr::startOffset() const {
    return theStartOffset;
}

int Dted_Hdr::stopOffset() const {
    return theStopOffset;
}

Dted_Hdr::Dted_Hdr(const Dted_Hdr& source) {
}

const Dted_Hdr& Dted_Hdr::operator=(const Dted_Hdr& rhs) {
    return rhs;
}

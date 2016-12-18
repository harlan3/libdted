//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Accuracy Description
//               (ACC) of a DTED file.
//
//********************************************************************

#include <stdio.h>
#include "Dted_Acc.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

Dted_Acc::Dted_Acc(const string& dted_file,
                   int offset)
   :
      theRecSen(),
      theAbsoluteCE(),
      theAbsoluteLE(),
      theRelativeCE(),
      theRelativeLE(),
      theField6(),
      theField7(),
      theField8(),
      theField9(),
      theField10(),
      theField11(),
      theField12(),
      theField13(),
      theField14(),
      theField15(),
      theField16(),
      theField17(),
      theStartOffset(offset),
      theStopOffset(0)
{
   
   // Open the dted file for reading.
   ifstream in(dted_file.c_str());
   if(!in)
   { 
      cerr << "WARNING Dted_Acc::Dted_Acc Can't open the DTED file: " << dted_file << endl;
      return;
   }
   
   parse(in);
}

Dted_Acc::Dted_Acc(istream& in,
                           int offset)
   :
      theRecSen(),
      theAbsoluteCE(),
      theAbsoluteLE(),
      theRelativeCE(),
      theRelativeLE(),
      theField6(),
      theField7(),
      theField8(),
      theField9(),
      theField10(),
      theField11(),
      theField12(),
      theField13(),
      theField14(),
      theField15(),
      theField16(),
      theField17(),
      theStartOffset(offset),
      theStopOffset(0)
{
   parse(in);
}

void Dted_Acc::parse(istream& in) {

    // Seek to the start of the record.
    in.seekg(theStartOffset, ios::beg);

    // Parse theRecSen
    in.read(theRecSen, FIELD1_SIZE);
    theRecSen[FIELD1_SIZE] = '\0';

    if (!(strncmp(theRecSen, "ACC", 3) == 0)) {
        cerr << "error in Dted_Acc::parse(istream& in)" << endl;
        return;
    }

    // Parse theAbsoluteCE
    in.read(theAbsoluteCE, FIELD2_SIZE);
    theAbsoluteCE[FIELD2_SIZE] = '\0';

    // Parse theAbsoluteLE
    in.read(theAbsoluteLE, FIELD3_SIZE);
    theAbsoluteLE[FIELD3_SIZE] = '\0';

    // Parse theRelativeCE
    in.read(theRelativeCE, FIELD4_SIZE);
    theRelativeCE[FIELD4_SIZE] = '\0';

    // Parse theRelativeLE
    in.read(theRelativeLE, FIELD5_SIZE);
    theRelativeLE[FIELD5_SIZE] = '\0';

    // Parse Field 6
    in.read(theField6, FIELD6_SIZE);
    theField6[FIELD6_SIZE] = '\0';

    // Parse Field 7
    in.read(theField7, FIELD7_SIZE);
    theField7[FIELD7_SIZE] = '\0';

    // Parse Field 8
    in.read(theField8, FIELD8_SIZE);
    theField8[FIELD8_SIZE] = '\0';

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

    // Parse Field 15
    in.read(theField15, FIELD15_SIZE);
    theField15[FIELD15_SIZE] = '\0';

    // Parse Field 16
    in.read(theField16, FIELD16_SIZE);
    theField16[FIELD16_SIZE] = '\0';

    // Parse Field 17
    in.read(theField17, FIELD17_SIZE);
    theField17[FIELD17_SIZE] = '\0';

    // Set the stop offset.
    theStopOffset = theStartOffset + ACC_LENGTH;
}

int Dted_Acc::absCE() const {
    return (theAbsoluteCE ? atoi(theAbsoluteCE) : 0);
}

int Dted_Acc::absLE() const {
    return (theAbsoluteLE ? atoi(theAbsoluteLE) : 0);
}

int Dted_Acc::relCE() const {
    return (theRelativeCE ? atoi(theRelativeCE) : 0);
}

int Dted_Acc::relLE() const {
    return (theRelativeLE ? atoi(theRelativeLE) : 0);
}

int Dted_Acc::startOffset() const {
    return theStartOffset;
}

int Dted_Acc::stopOffset() const {
    return theStopOffset;
}

ostream& operator<<(ostream& os, const Dted_Acc& acc) {
    os << "\nDTED Header (ACC):" << "\n-------------------------------"
            << "\nRecoginition Sentinel: " << acc.theRecSen
            << "\nAbsolute CE:           " << acc.theAbsoluteCE
            << "\nAbsolute LE:           " << acc.theAbsoluteLE
            << "\nRelative CE:           " << acc.theRelativeCE
            << "\nRelative LE:           " << acc.theRelativeLE
            << "\nStart Offset:          " << acc.theStartOffset
            << "\nStop Offset:           " << acc.theStopOffset << endl;

    return os;
}

Dted_Acc::Dted_Acc(const Dted_Acc& source) {
}

const Dted_Acc& Dted_Acc::operator=(const Dted_Acc& rhs) {
    return rhs;
}

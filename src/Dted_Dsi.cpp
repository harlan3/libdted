//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class provides access to the Data Set Identification
//               (DSI) of a DTED file.
//
//********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "Dted_Dsi.h"

Dted_Dsi::Dted_Dsi(const string& dted_file, int offset)
   :
      theRecSen(),
      theSecurityCode(),
      theField3(),
      theField4(),
      theProductLevel(),
      theField7(),
      theField8(),
      theEdition(),
      theMatchMergeVersion(),
      theMaintenanceDate(),
      theMatchMergeDate(),
      theMaintenanceCode(),
      theProducerCode(),
      theField15(),
      theProductStockSpecNumber(),
      theProductSpecNumber(),
      theProductSpecDate(),
      theVerticalDatum(),
      theHorizontalDatum(),
      theField21(),
      theCompilationDate(),
      theField23(),
      theLatOrigin(),
      theLonOrigin(),
      theLatSW(),
      theLonSW(),
      theLatNW(),
      theLonNW(),
      theLatNE(),
      theLonNE(),
      theLatSE(),
      theLonSE(),
      theOrientation(),
      theLatInterval(),
      theLonInterval(),
      theNumLatPoints(),
      theNumLonLines(),
      theCellIndicator(),
      theField40(),
      theField41(),
      theField42(),
      theStartOffset(offset),
      theStopOffset(0)
{

    // Open the dted file for reading.
    ifstream in(dted_file.c_str());
    if (!in) {
        cerr << "FATAL Dted_Dsi::Dted_Dsi: Unable to open the DTED file: "
                << dted_file << std::endl;
        return;
    }

    parse(in);

    in.close();
}

Dted_Dsi::Dted_Dsi(istream& in, int offset) :
        theRecSen(), theSecurityCode(), theField3(), theField4(), theProductLevel(), theField7(), theField8(), theEdition(), theMatchMergeVersion(), theMaintenanceDate(), theMatchMergeDate(), theMaintenanceCode(), theProducerCode(), theField15(), theProductStockSpecNumber(), theProductSpecNumber(), theProductSpecDate(), theVerticalDatum(), theHorizontalDatum(), theField21(), theCompilationDate(), theField23(), theLatOrigin(), theLonOrigin(), theLatSW(), theLonSW(), theLatNW(), theLonNW(), theLatNE(), theLonNE(), theLatSE(), theLonSE(), theOrientation(), theLatInterval(), theLonInterval(), theNumLatPoints(), theNumLonLines(), theCellIndicator(), theField40(), theField41(), theField42(), theStartOffset(
                offset), theStopOffset(0) {
    parse(in);
}

void Dted_Dsi::parse(istream& in) {

    char tmp_chars[26]; // For blank field reads.

    // Seek to the start of the record.
    in.seekg(theStartOffset, ios::beg);

    // Parse theRecSen
    in.read(theRecSen, FIELD1_SIZE);
    theRecSen[FIELD1_SIZE] = '\0';

    if (!(strncmp(theRecSen, "DSI", 3) == 0)) {
        cerr << "Error in Dted_Dsi::parse(istream& in)" << endl;
        return;
    }

    // Parse theSecurityCode
    in.read(theSecurityCode, FIELD2_SIZE);
    theSecurityCode[FIELD2_SIZE] = '\0';

    // Parse Field 3
    in.read(theField3, FIELD3_SIZE);
    theField3[FIELD3_SIZE] = '\0';

    // Parse Field 4
    in.read(theField4, FIELD4_SIZE);
    theField4[FIELD4_SIZE] = '\0';

    // Parse Field 5 (currently blank)
    in.read(tmp_chars, FIELD5_SIZE);

    // Parse theProductLevel
    in.read(theProductLevel, FIELD6_SIZE);
    theProductLevel[FIELD6_SIZE] = '\0';

    // Parse Field 7
    in.read(theField7, FIELD7_SIZE);
    theField7[FIELD7_SIZE] = '\0';

    // Parse Field 8
    in.read(theField8, FIELD8_SIZE);
    theField8[FIELD8_SIZE] = '\0';

    // Parse theEdition
    in.read(theEdition, FIELD9_SIZE);
    theEdition[FIELD9_SIZE] = '\0';

    // Parse theMatchMergeVersion
    in.read(theMatchMergeVersion, FIELD10_SIZE);
    theMatchMergeVersion[FIELD10_SIZE] = '\0';

    // Parse theMaintenanceDate
    in.read(theMaintenanceDate, FIELD11_SIZE);
    theMaintenanceDate[FIELD11_SIZE] = '\0';

    // Parse theMatchMergeDate
    in.read(theMatchMergeDate, FIELD12_SIZE);
    theMatchMergeDate[FIELD12_SIZE] = '\0';

    // Parse theMaintenanceCode
    in.read(theMaintenanceCode, FIELD13_SIZE);
    theMaintenanceCode[FIELD13_SIZE] = '\0';

    // Parse theProducerCode
    in.read(theProducerCode, FIELD14_SIZE);
    theProducerCode[FIELD14_SIZE] = '\0';

    // Parse Field 15
    in.read(theField15, FIELD15_SIZE);
    theField15[FIELD15_SIZE] = '\0';

    // Parse theProductStockSpecNumber
    in.read(theProductStockSpecNumber, FIELD16_SIZE);
    theProductStockSpecNumber[FIELD16_SIZE] = '\0';

    // Parse theProductSpecNumber
    in.read(theProductSpecNumber, FIELD17_SIZE);
    theProductSpecNumber[FIELD17_SIZE] = '\0';

    // Parse theProductSpecDate
    in.read(theProductSpecDate, FIELD18_SIZE);
    theProductSpecDate[FIELD18_SIZE] = '\0';

    // Parse theVerticalDatum
    in.read(theVerticalDatum, FIELD19_SIZE);
    theVerticalDatum[FIELD19_SIZE] = '\0';

    // Parse theHorizontalDatum
    in.read(theHorizontalDatum, FIELD20_SIZE);
    theHorizontalDatum[FIELD20_SIZE] = '\0';

    // Parse Field 21
    in.read(theField21, FIELD21_SIZE);
    theField21[FIELD21_SIZE] = '\0';

    // Parse theCompilationDate
    in.read(theCompilationDate, FIELD22_SIZE);
    theCompilationDate[FIELD22_SIZE] = '\0';

    // Parse Field 23
    in.read(theField23, FIELD23_SIZE);
    theField23[FIELD23_SIZE] = '\0';

    // Parse theLatOrigin
    in.read(theLatOrigin, FIELD24_SIZE);
    theLatOrigin[FIELD24_SIZE] = '\0';

    // Parse theLonOrigin
    in.read(theLonOrigin, FIELD25_SIZE);
    theLonOrigin[FIELD25_SIZE] = '\0';

    // Parse theLatSW
    in.read(theLatSW, FIELD26_SIZE);
    theLatSW[FIELD26_SIZE] = '\0';

    // Parse theLonSW
    in.read(theLonSW, FIELD27_SIZE);
    theLonSW[FIELD27_SIZE] = '\0';

    // Parse theLatNW
    in.read(theLatNW, FIELD28_SIZE);
    theLatNW[FIELD28_SIZE] = '\0';

    // Parse theLonNW
    in.read(theLonNW, FIELD29_SIZE);
    theLonNW[FIELD29_SIZE] = '\0';

    // Parse theLatNE
    in.read(theLatNE, FIELD30_SIZE);
    theLatNE[FIELD30_SIZE] = '\0';

    // Parse theLonNE
    in.read(theLonNE, FIELD31_SIZE);
    theLonNE[FIELD31_SIZE] = '\0';

    // Parse theLatSE
    in.read(theLatSE, FIELD32_SIZE);
    theLatSE[FIELD32_SIZE] = '\0';

    // Parse theLonSE
    in.read(theLonSE, FIELD33_SIZE);
    theLonSE[FIELD33_SIZE] = '\0';

    // Parse theOrientation
    in.read(theOrientation, FIELD34_SIZE);
    theOrientation[FIELD34_SIZE] = '\0';

    // Parse theLatInterval
    in.read(theLatInterval, FIELD35_SIZE);
    theLatInterval[FIELD35_SIZE] = '\0';

    // Parse theLonInterval
    in.read(theLonInterval, FIELD36_SIZE);
    theLonInterval[FIELD36_SIZE] = '\0';

    // Parse theNumLatPoints
    in.read(theNumLatPoints, FIELD37_SIZE);
    theNumLatPoints[FIELD37_SIZE] = '\0';

    // Parse theNumLonLines
    in.read(theNumLonLines, FIELD38_SIZE);
    theNumLonLines[FIELD38_SIZE] = '\0';

    // Parse theCellIndicator
    in.read(theCellIndicator, FIELD39_SIZE);
    theCellIndicator[FIELD39_SIZE] = '\0';

    // Parse Field 40
    in.read(theField40, FIELD40_SIZE);
    theField40[FIELD40_SIZE] = '\0';

    // Parse Field 41
    in.read(theField41, FIELD41_SIZE);
    theField41[FIELD41_SIZE] = '\0';

    // Parse Field 42
    in.read(theField42, FIELD42_SIZE);
    theField42[FIELD42_SIZE] = '\0';

    // Set the stop offset.
    theStopOffset = theStartOffset + DSI_LENGTH;
}

string Dted_Dsi::recoginitionSentinel() const {
    return theRecSen;
}

string Dted_Dsi::securityCode() const {
    return string(theSecurityCode);
}

string Dted_Dsi::productLevel() const {
    return string(theProductLevel);
}

string Dted_Dsi::edition() const {
    return string(theEdition);
}

string Dted_Dsi::matchMergeVersion() const {
    return string(theMatchMergeVersion);
}

string Dted_Dsi::maintanenceDate() const {
    return string(theMaintenanceDate);
}

string Dted_Dsi::matchMergeDate() const {
    return string(theMatchMergeDate);
}

string Dted_Dsi::maintenanceCode() const {
    return string(theMaintenanceCode);
}

string Dted_Dsi::producerCode() const {
    return string(theProducerCode);
}

string Dted_Dsi::productStockSpecNumber() const {
    return string(theProductStockSpecNumber);
}

string Dted_Dsi::productSpecNumber() const {
    return string(theProductSpecNumber);
}

string Dted_Dsi::productSpecDate() const {
    return string(theProductSpecDate);
}

string Dted_Dsi::verticalDatum() const {
    return string(theVerticalDatum);
}

string Dted_Dsi::horizontalDatum() const {
    return string(theHorizontalDatum);
}

string Dted_Dsi::compilationDate() const {
    return string(theCompilationDate);
}

string Dted_Dsi::latOrigin() const {
    return string(theLatOrigin);
}

string Dted_Dsi::lonOrigin() const {
    return string(theLonOrigin);
}

string Dted_Dsi::latSW() const {
    return string(theLatSW);
}

string Dted_Dsi::lonSW() const {
    return string(theLonSW);
}

string Dted_Dsi::latNW() const {
    return string(theLatNW);
}

string Dted_Dsi::lonNW() const {
    return string(theLonNW);
}

string Dted_Dsi::latNE() const {
    return string(theLatNE);
}

string Dted_Dsi::lonNE() const {
    return string(theLonNE);
}

string Dted_Dsi::latSE() const {
    return string(theLatSE);
}

string Dted_Dsi::lonSE() const {
    return string(theLonSE);
}

string Dted_Dsi::orientation() const {
    return string(theOrientation);
}

string Dted_Dsi::latInterval() const {
    return string(theLatInterval);
}

string Dted_Dsi::lonInterval() const {
    return string(theLonInterval);
}

int Dted_Dsi::numLatPoints() const {
    return (theNumLatPoints ? atoi(theNumLatPoints) : 0);
}

int Dted_Dsi::numLonLines() const {
    return (theNumLonLines ? atoi(theNumLonLines) : 0);
}

int Dted_Dsi::cellIndicator() const {
    return (theCellIndicator ? atoi(theCellIndicator) : 0);
}

int Dted_Dsi::startOffset() const {
    return theStartOffset;
}
int Dted_Dsi::stopOffset() const {
    return theStopOffset;
}

ostream& operator<<(ostream& os, const Dted_Dsi& dsi) {
    os << "\nDTED Header (DSI):" << "\n-------------------------------"
            << "\nRecoginition Sentinel:     " << dsi.theRecSen
            << "\nSecurity Code:             " << dsi.theSecurityCode
            << "\nProduct Level:             " << dsi.theProductLevel
            << "\nEdition:                   " << dsi.theEdition
            << "\nMatch/Merge Version:       " << dsi.theMatchMergeVersion
            << "\nMaintenance Date:          " << dsi.theMaintenanceDate
            << "\nMatch/Merge Date:          " << dsi.theMatchMergeDate
            << "\nMaintenance Code:          " << dsi.theMaintenanceCode
            << "\nProducer Code:             " << dsi.theProducerCode
            << "\nProduct Stock Spec Number: " << dsi.theProductStockSpecNumber
            << "\nProduct Spec Number:       " << dsi.theProductSpecNumber
            << "\nVertical Datum:            " << dsi.theVerticalDatum
            << "\nHorizontal Datum:          " << dsi.theHorizontalDatum
            << "\nCompilation Date:          " << dsi.theCompilationDate
            << "\nLat Origin:                " << dsi.theLatOrigin
            << "\nLon Origin:                " << dsi.theLonOrigin
            << "\nLat SW:                    " << dsi.theLatSW
            << "\nLon SW:                    " << dsi.theLonSW
            << "\nLat NW:                    " << dsi.theLatNW
            << "\nLon NW:                    " << dsi.theLonNW
            << "\nLat NE:                    " << dsi.theLatNE
            << "\nLon NE:                    " << dsi.theLonNE
            << "\nLat SE:                    " << dsi.theLatSE
            << "\nLon SE:                    " << dsi.theLonSE
            << "\nOrientation:               " << dsi.theOrientation
            << "\nLat Interval:              " << dsi.theLatInterval
            << "\nLon Interval:              " << dsi.theLonInterval
            << "\nNumber of Lat Points:      " << dsi.theNumLatPoints
            << "\nNumber of Lon Lines:       " << dsi.theNumLonLines
            << "\nCell Indicator:            " << dsi.theCellIndicator
            << "\nStart Offset:              " << dsi.theStartOffset
            << "\nStop Offset:               " << dsi.theStopOffset << endl;

    return os;
}

Dted_Dsi::Dted_Dsi(const Dted_Dsi& source) {
}

const Dted_Dsi& Dted_Dsi::operator=(const Dted_Dsi& rhs) {
    return rhs;
}

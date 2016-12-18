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

#ifndef Dted_Dsi_H
#define Dted_Dsi_H
#include <iostream>
#include <string.h>

using namespace std;

class Dted_Dsi {
public:

    //! Dted Dsi constructor.
    Dted_Dsi(const string& dted_file, int offset);

    //! Dted Dsi constructor.
    Dted_Dsi(std::istream& in, int offset);

    enum {
        DSI_LENGTH = 648,
        DSI_SECURITY_CODE = 4,
        DSI_SECURITY_CONTROL = 5,
        DSI_SECURITY_HANDLING = 7,
        DSI_RESERVED_1 = 34,
        DSI_DMA_SERIES = 60,
        DSI_RESERVED_2 = 65,
        DSI_RESERVED_3 = 80,
        DSI_DATA_EDITION = 88,
        DSI_MATCH_MERGE_VERSION = 90,
        DSI_MAINTENANCE_DATE = 91,
        DSI_MATCH_MERGE_DATE = 95,
        DSI_MAINTENANCE_CODE = 99,
        DSI_PRODUCER_CODE = 103,
        DSI_RESERVED_4 = 111,
        DSI_PRODUCT_SPEC_STOCK_NUMBER = 127,
        DSI_PRODUCT_SPEC_NUMBER = 136,
        DSI_PRODUCT_SPEC_DATE = 138,
        DSI_VERTICAL_DATUM = 142,
        DSI_HORIZ_DATUM = 145,
        DSI_DIGITIZING_SYSTEM = 150,
        DSI_COMPILATION_DATE = 160,
        DSI_RESERVED_5 = 164,
        DSI_LAT_ORIGIN = 186,
        DSI_LON_ORIGIN = 195,
        DSI_LAT_SW = 205,
        DSI_LON_SW = 212,
        DSI_LAT_NW = 220,
        DSI_LON_NW = 227,
        DSI_LAT_NE = 235,
        DSI_LON_NE = 242,
        DSI_LAT_SE = 250,
        DSI_LON_SE = 257,
        DSI_ORIENTATION = 265,
        DSI_LAT_INTERVAL = 274,
        DSI_LON_INTERVAL = 278,
        DSI_NUM_LAT_LINES = 282,
        DSI_NUM_LON_LINES = 286,
        DSI_PARTIAL_CELL_INDICATOR = 290,
        DSI_RESERVED = 292,
        FIELD1_SIZE = 3,
        FIELD2_SIZE = 1,
        FIELD3_SIZE = 2,
        FIELD4_SIZE = 27,
        FIELD5_SIZE = 26,
        FIELD6_SIZE = 5,
        FIELD7_SIZE = 15,
        FIELD8_SIZE = 8,
        FIELD9_SIZE = 2,
        FIELD10_SIZE = 1,
        FIELD11_SIZE = 4,
        FIELD12_SIZE = 4,
        FIELD13_SIZE = 4,
        FIELD14_SIZE = 8,
        FIELD15_SIZE = 16,
        FIELD16_SIZE = 9,
        FIELD17_SIZE = 2,
        FIELD18_SIZE = 4,
        FIELD19_SIZE = 3,
        FIELD20_SIZE = 5,
        FIELD21_SIZE = 10,
        FIELD22_SIZE = 4,
        FIELD23_SIZE = 22,
        FIELD24_SIZE = 9,
        FIELD25_SIZE = 10,
        FIELD26_SIZE = 7,
        FIELD27_SIZE = 8,
        FIELD28_SIZE = 7,
        FIELD29_SIZE = 8,
        FIELD30_SIZE = 7,
        FIELD31_SIZE = 8,
        FIELD32_SIZE = 7,
        FIELD33_SIZE = 8,
        FIELD34_SIZE = 9,
        FIELD35_SIZE = 4,
        FIELD36_SIZE = 4,
        FIELD37_SIZE = 4,
        FIELD38_SIZE = 4,
        FIELD39_SIZE = 2,
        FIELD40_SIZE = 101,
        FIELD41_SIZE = 100,
        FIELD42_SIZE = 156
    };

    //! The Recoginition Sentinel signifies if the DSI record exists.
    string recoginitionSentinel() const;

    //! Return the securityCode from dted cell.
    string securityCode() const;

    //! Return the productLevel from dted cell.
    string productLevel() const;

    //! Return the edition from dted cell.
    string edition() const;

    //! Return the matchMergeVersion from dted cell.
    string matchMergeVersion() const;

    //! Return the maintanenceDate from dted cell.
    string maintanenceDate() const;

    //! Return the matchMergeDate from dted cell.
    string matchMergeDate() const;

    //! Return the maintenanceCode from dted cell.
    string maintenanceCode() const;

    //! Return the producerCode from dted cell.
    string producerCode() const;

    //! Return the productStockSpecNumber from dted cell.
    string productStockSpecNumber() const;

    //! Return the productSpecNumber from dted cell.
    string productSpecNumber() const;

    //! Return the productSpecDate from dted cell.
    string productSpecDate() const;

    //! Return the verticalDatum from dted cell.
    string verticalDatum() const;

    //! Return the horizontalDatum from dted cell.
    string horizontalDatum() const;

    //! Return the compilationDate from dted cell.
    string compilationDate() const;

    //! Return the latOrigin from dted cell.
    string latOrigin() const;

    //! Return the lonOrigin from dted cell.
    string lonOrigin() const;

    //! Return the latSW from dted cell.
    string latSW() const;

    //! Return the lonSW from dted cell.
    string lonSW() const;

    //! Return the latNW from dted cell.
    string latNW() const;

    //! Return the lonNW from dted cell.
    string lonNW() const;

    //! Return the latNE from dted cell.
    string latNE() const;

    //! Return the lonNE from dted cell.
    string lonNE() const;

    //! Return the latSE from dted cell.
    string latSE() const;

    //! Return the lonSE from dted cell.
    string lonSE() const;

    //! Return the orientation from dted cell.
    string orientation() const;

    //! Return the latInterval from dted cell.
    string latInterval() const;

    //! Return the lonInterval from dted cell.
    string lonInterval() const;

    //! Return the numLatPoints from dted cell.
    int numLatPoints() const;

    //! Return the numLonLines from dted cell.
    int numLonLines() const;

    //! Return the cellIndicator from dted cell.
    int cellIndicator() const;

    //! Return the startOffset from dted cell.
    int startOffset() const;

    //! Return the stopOffset from dted cell.
    int stopOffset() const;

    friend std::ostream& operator<<(std::ostream& os, const Dted_Dsi& dsi);

    //! Return the securityCode from dted cell.
    void parse(std::istream& in);

private:
    // Do not allow...
    Dted_Dsi(const Dted_Dsi& source);
    const Dted_Dsi& operator=(const Dted_Dsi& rhs);

    char theRecSen[FIELD1_SIZE + 1];
    char theSecurityCode[FIELD2_SIZE + 1];
    char theField3[FIELD3_SIZE + 1];
    char theField4[FIELD4_SIZE + 1];
    char theProductLevel[FIELD6_SIZE + 1];
    char theField7[FIELD7_SIZE + 1];
    char theField8[FIELD8_SIZE + 1];
    char theEdition[FIELD9_SIZE + 1];
    char theMatchMergeVersion[FIELD10_SIZE + 1];
    char theMaintenanceDate[FIELD11_SIZE + 1];
    char theMatchMergeDate[FIELD12_SIZE + 1];
    char theMaintenanceCode[FIELD13_SIZE + 1];
    char theProducerCode[FIELD14_SIZE + 1];
    char theField15[FIELD15_SIZE + 1];
    char theProductStockSpecNumber[FIELD16_SIZE + 1];
    char theProductSpecNumber[FIELD17_SIZE + 1];
    char theProductSpecDate[FIELD18_SIZE + 1];
    char theVerticalDatum[FIELD19_SIZE + 1];
    char theHorizontalDatum[FIELD20_SIZE + 1];
    char theField21[FIELD21_SIZE + 1];
    char theCompilationDate[FIELD22_SIZE + 1];
    char theField23[FIELD23_SIZE + 1];
    char theLatOrigin[FIELD24_SIZE + 1];
    char theLonOrigin[FIELD25_SIZE + 1];
    char theLatSW[FIELD26_SIZE + 1];
    char theLonSW[FIELD27_SIZE + 1];
    char theLatNW[FIELD28_SIZE + 1];
    char theLonNW[FIELD29_SIZE + 1];
    char theLatNE[FIELD30_SIZE + 1];
    char theLonNE[FIELD31_SIZE + 1];
    char theLatSE[FIELD32_SIZE + 1];
    char theLonSE[FIELD33_SIZE + 1];
    char theOrientation[FIELD34_SIZE + 1];
    char theLatInterval[FIELD35_SIZE + 1];
    char theLonInterval[FIELD36_SIZE + 1];
    char theNumLatPoints[FIELD37_SIZE + 1];
    char theNumLonLines[FIELD38_SIZE + 1];
    char theCellIndicator[FIELD39_SIZE + 1];
    char theField40[FIELD40_SIZE + 1];
    char theField41[FIELD41_SIZE + 1];
    char theField42[FIELD42_SIZE + 1];

    int theStartOffset;
    int theStopOffset;
};

#endif

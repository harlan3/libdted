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

#ifndef DtedHdr_H
#define DtedHdr_H
#include <iostream>
#include <string>

using namespace std;

class Dted_Hdr {
public:

    //! Dted Hdr constructor.
    Dted_Hdr(const string& dted_file, int offset);

    //! Dted Hdr constructor.
    Dted_Hdr(std::istream& in, int offset);

    enum {
        HDR_LENGTH = 80,
        HDR_ONE_LABEL_1 = 4,
        HDR_FILENAME = 5,
        HDR_UNIVAC = 22,
        HDR_REEL = 28,
        HDR_FILE_NUM = 32,
        HDR_GENERATION_NUM = 36,
        HDR_VERSION_NUM = 40,
        HDR_CREATION_DATE = 42,
        HDR_EXPIRATION_DATE = 48,
        HDR_ACCESS = 54,
        HDR_BLOCK_COUNT = 55,
        HDR_QUALIFIER = 61,
        HDR_BLANKS = 74,
        FIELD1_SIZE = 3,
        FIELD2_SIZE = 1,
        FIELD3_SIZE = 17,
        FIELD4_SIZE = 6,
        FIELD5_SIZE = 4,
        FIELD6_SIZE = 4,
        FIELD7_SIZE = 4,
        FIELD8_SIZE = 2,
        FIELD9_SIZE = 6,
        FIELD10_SIZE = 6,
        FIELD11_SIZE = 1,
        FIELD12_SIZE = 6,
        FIELD13_SIZE = 13,
        FIELD14_SIZE = 7
    };

    //! The Recoginition Sentinel signifies if the HDR record exists.
    string recoginitionSentinel() const;

    //! Return the filename from Dted header.
    string fileName() const;

    //! Return the version from Dted header.
    string version() const;

    //! Return the creation date from Dted header.
    string creationDate() const;

    //! Return the start offset from Dted header.
    int startOffset() const;

    //! Return the stop offset from Dted header.
    int stopOffset() const;

    friend std::ostream& operator<<(std::ostream& os, const Dted_Hdr& hdr);

    //! Parse the dted header.
    void parse(std::istream& in);

private:
    // Do not allow...
    Dted_Hdr(const Dted_Hdr& source);
    const Dted_Hdr& operator=(const Dted_Hdr& rhs);

    char theRecSen[FIELD1_SIZE + 1];
    char theField2[FIELD2_SIZE + 1];
    char theFilename[FIELD3_SIZE + 1];
    char theField4[FIELD4_SIZE + 1];
    char theField5[FIELD5_SIZE + 1];
    char theField6[FIELD6_SIZE + 1];
    char theVersion[FIELD7_SIZE + 1];
    char theCreationDate[FIELD8_SIZE + 1];
    char theField9[FIELD9_SIZE + 1];
    char theField10[FIELD10_SIZE + 1];
    char theField11[FIELD11_SIZE + 1];
    char theField12[FIELD12_SIZE + 1];
    char theField13[FIELD13_SIZE + 1];
    char theField14[FIELD14_SIZE + 1];

    int theStartOffset;
    int theStopOffset;
};

#endif

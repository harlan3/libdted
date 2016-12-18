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

#ifndef Dted_Vol_H
#define Dted_Vol_H
#include <iostream>
#include <string>

class Dted_Vol {
public:

    //! Dted_Vol constructor
    Dted_Vol(const string& dted_file, int offset);

    //! Dted_Vol constructor
    Dted_Vol(std::istream& in, int offset);

    enum {
        VOL_LENGTH = 80,
        VOL_ONE_LABEL_1 = 4,
        VOL_REEL_NUMBER = 5,
        VOL_REEL_ACCESS = 11,
        VOL_SPACE = 12,
        VOL_ACCOUNT_NUMBER = 38,
        VOL_BLANKS = 52,
        VOL_ONE_LABEL_2 = 80,
        FIELD1_SIZE = 3,
        FIELD2_SIZE = 1,
        FIELD3_SIZE = 6,
        FIELD4_SIZE = 1,
        FIELD5_SIZE = 26,
        FIELD6_SIZE = 14,
        FIELD7_SIZE = 28,
        FIELD8_SIZE = 1
    };

    //! The Recoginition Sentinel signifies if the VOL record exists.
    string getRecoginitionSentinel() const;

    //! Return the ReelNumber.
    string getReelNumber() const;

    //! Return the AccountNumber.
    string getAccountNumber() const;

    //! Return the startOffset.
    int startOffset() const;

    //! Return the stopOffset.
    int stopOffset() const;
    friend std::ostream& operator<<(std::ostream& os, const Dted_Vol& vol);

    //! Parse the Dted Vol.
    void parse(std::istream& in);

private:
    // Do not allow...
    Dted_Vol(const Dted_Vol& source);
    const Dted_Vol& operator=(const Dted_Vol& rhs);

    char theRecSen[FIELD1_SIZE + 1];
    char theField2[FIELD2_SIZE + 1];
    char theReelNumber[FIELD3_SIZE + 1];
    char theField4[FIELD4_SIZE + 1];
    char theField5[FIELD5_SIZE + 1];
    char theAccountNumber[FIELD6_SIZE + 1];
    char theField7[FIELD7_SIZE + 1];
    char theField8[FIELD8_SIZE + 1];

    int theStartOffset;
    int theStopOffset;
};

#endif

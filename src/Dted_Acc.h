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

#ifndef Dted_Acc_H
#define Dted_Acc_H

#include <iostream>
#include <string.h>

using namespace std;

class Dted_Acc {
public:

    //! Constructor for DTED accuracy description file.
    Dted_Acc(const string& dted_file, int offset);

    //! Constructor for DTED accuracy description file.
    Dted_Acc(std::istream& in, int offset);

    enum {
        ACC_LENGTH = 2700,
        ACC_ABSOLUTE_CE = 4,
        ACC_ABSOLUTE_LE = 8,
        ACC_RELATIVE_CE = 12,
        ACC_RELATIVE_LE = 16,
        ACC_RESERVED_1 = 20,
        ACC_RESERVED_2 = 24,
        ACC_RESERVED_3 = 25,
        ACC_MULTIPLE_FLAG = 56,
        ACC_RESERVED = 58,
        FIELD1_SIZE = 3,
        FIELD2_SIZE = 4,
        FIELD3_SIZE = 4,
        FIELD4_SIZE = 4,
        FIELD5_SIZE = 4,
        FIELD6_SIZE = 4,
        FIELD7_SIZE = 1,
        FIELD8_SIZE = 31,
        FIELD9_SIZE = 2,
        FIELD10_SIZE = 4,
        FIELD11_SIZE = 4,
        FIELD12_SIZE = 4,
        FIELD13_SIZE = 4,
        FIELD14_SIZE = 2,
        FIELD15_SIZE = 9,
        FIELD16_SIZE = 10,
        FIELD17_SIZE = 18,
        FIELD18_SIZE = 69
    };

    //! Return the absolute circular error.
    int absCE() const;

    //! Return the absolute linear error.
    int absLE() const;

    //! Return the relative circular error.
    int relCE() const;

    //! Return the relative linear error.
    int relLE() const;

    //! Return the start offset
    int startOffset() const;

    //! Return the stop offset
    int stopOffset() const;

    friend std::ostream& operator<<(std::ostream& os, const Dted_Acc& acc);

    //! Parse the Dted Acc.
    void parse(std::istream& in);

private:

    // Do not allow...
    Dted_Acc(const Dted_Acc& source);
    const Dted_Acc& operator=(const Dted_Acc& rhs);

    char theRecSen[FIELD1_SIZE + 1];
    char theAbsoluteCE[FIELD2_SIZE + 1];
    char theAbsoluteLE[FIELD3_SIZE + 1];
    char theRelativeCE[FIELD4_SIZE + 1];
    char theRelativeLE[FIELD5_SIZE + 1];
    char theField6[FIELD6_SIZE + 1];
    char theField7[FIELD7_SIZE + 1];
    char theField8[FIELD8_SIZE + 1];
    char theField9[FIELD9_SIZE + 1];
    char theField10[FIELD10_SIZE + 1];
    char theField11[FIELD11_SIZE + 1];
    char theField12[FIELD12_SIZE + 1];
    char theField13[FIELD13_SIZE + 1];
    char theField14[FIELD14_SIZE + 1];
    char theField15[FIELD15_SIZE + 1];
    char theField16[FIELD16_SIZE + 1];
    char theField17[FIELD17_SIZE + 1];

    int theStartOffset;
    int theStopOffset;
};

#endif

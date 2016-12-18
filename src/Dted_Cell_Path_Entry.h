//*******************************************************************
// Copyright (C) 2006 Free Software Foundation
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Harlan Murphy
//
// Description:  This class provides Dted cell path comparisons
//
//********************************************************************

#ifndef Dted_Cell_Path_Entry_H
#define Dted_Cell_Path_Entry_H

#include <string>

using namespace std;

class Dted_Cell_Path_Entry {
public:

    //! DTED cell latitude.
    short int latitude;

    //! DTED cell longitude.
    short int longitude;

    //! DTED cell path.
    string cellPath;

    bool operator <(
            const Dted_Cell_Path_Entry& other_Dted_Cell_Path_Entry) const;

    int operator ==(
            const Dted_Cell_Path_Entry& other_Dted_Cell_Path_Entry) const;
};

#endif

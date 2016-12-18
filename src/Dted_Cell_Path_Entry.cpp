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

#include "Dted_Cell_Path_Entry.h"

using namespace std;

bool Dted_Cell_Path_Entry::operator <(
        const Dted_Cell_Path_Entry& other_Dted_Cell_Path_Entry) const {
    if (latitude < other_Dted_Cell_Path_Entry.latitude)
        return true;
    else if (other_Dted_Cell_Path_Entry.latitude > latitude)
        return false;

    if (longitude < other_Dted_Cell_Path_Entry.longitude)
        return true;

    return false;
}

int Dted_Cell_Path_Entry::operator ==(
        const Dted_Cell_Path_Entry& other_Dted_Cell_Path_Entry) const {
    if ((latitude == other_Dted_Cell_Path_Entry.latitude)
            && (longitude == other_Dted_Cell_Path_Entry.longitude))
        return true;
    else
        return false;
}


//*******************************************************************
// Copyright (C) 2006 Free Software Foundation
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Harlan Murphy
//
// Description:  Implements the DTED lookup directory for retrieving
//               DTED cell entry information.
//
//********************************************************************

#ifndef Dted_Directory_H
#define Dted_Directory_H

#include <iostream>
#include <set>

#include "Dted_Cell_Path_Entry.h"

using namespace std;

class Dted_Directory {
public:

    //! Default constructor.
    Dted_Directory();

    //! Populates the Dted_Directory with DTED entries.  The DTED entries
    //! are generated from the path passed in as an input variable.
    bool Populate_Directory(const string& path);

    //! Displays all of the entries in the DTED directory.
    void Dump_Path_Entry_Set();

    //! Retireve a DTED entry based on latitude and longitude.
    bool Retrieve_Dted_Entry(Dted_Cell_Path_Entry &dted_Cell_Path_Entry);

    //! Reset the query iterator.
    void QueryReset(void);

    //! Query the DTED directory for cell path entries
    bool Query(Dted_Cell_Path_Entry &dtedCellPathEntry);

    //! Clear the contents of the Dted Directory
    void Clear_Dted_Directory();

    //! Return the minimum meridian for the Minimun Bounding Rectangle(MBR)
    string getMinMeridian();

    //! Return the maxmimum meridian for the Minimun Bounding Rectangle(MBR)
    string getMaxMeridian();

    //! Return the minimum parallel for the Minimun Bounding Rectangle(MBR)
    string getMinParallel();

    //! Return the maxmimum parallel for the Minimun Bounding Rectangle(MBR)
    string getMaxParallel();

private:
    //! Returns true if a filename contains a DTED extension.
    bool Is_Dted_File(string fileName);
    short currPathMeridian;
    string minMeridian;
    string maxMeridian;
    string minParallel;
    string maxParallel;
    typedef std::set<Dted_Cell_Path_Entry> Path_Entry_Set;

    Path_Entry_Set::const_iterator querytIt;

    //! STL Set container for DTED Cell Path entries.
    Path_Entry_Set pathEntrySet;

    bool debug;
};

#endif

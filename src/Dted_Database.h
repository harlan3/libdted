//*******************************************************************
// Copyright (C) 2006 Free Software Foundation
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Harlan Murphy
//
// Description:  Contains implementation of Dted_Database providing
//               access to dted library
//
//********************************************************************

#ifndef Dted_Database_H
#define Dted_Database_H

#include <set>
#include "Dted_Cell.h"
#include "Dted_Common.h"
#include "Dted_Directory.h"

using namespace std;

class Dted_Database {
public:

    Dted_Database();

    //! Clear the contents of the Dted database.
    void Clear_Database();

    /*! Populate Dted1 Directory with option to preload.
     @param path directory path of Dted1 data.
     @param preload Dted1 data into memory for fast access.
     @return true if successful.
     */
    bool Populate_Dted1_Directory(const string& path, bool preLoad);

    /*! Populate Dted2 Directory with option to preload.
     @param path directory path of Dted1 data.
     @param preload Dted2 data into memory for fast access.
     @return true if successful.
     */
    bool Populate_Dted2_Directory(const string& path, bool preLoad);

    //! Retrieve a Dted geolocation elevation based on the current Dted
    //! level and access method.
    double Get_Geo_Elev(Geo_Location geoLoc);

    //! Retrieve a Dted post.
    double Get_Post_Elev(Geo_Location geoLoc, Voxel pointLoc);

    //! Gather statistics for Dted1 Directory
    void Gather_Stats_Dted1();

    //! Gather statistics for Dted2 Directory
    void Gather_Stats_Dted2();

    //! Enable/Disable bilinear intepolation
    void Set_Bilinear_Interp_Active(bool newState);

    //! Set the Access_Method for DTED data to either DISK_ACCESS or
    //! MEMORY_ACCESS.
    void Set_Access_Method(Access_Method newMethod);

    //! Retrieves the Access_Method for DTED data.  Returns either
    //! DISK_ACCESS or MEMORY_ACCESS.
    Access_Method Get_Access_Method() const;

    //! Set the current DTED level
    void Set_Dted_Level(Dted_Level newDtedLevel);

    //! Returns the current DTED level
    Dted_Level Get_Dted_Level();

private:

    Dted_Directory dted1_dir;
    Dted_Directory dted2_dir;

    //! The current dted level.
    Dted_Level dtedLevel;

    typedef std::set<Dted_Cell*> Dted_Cell_Set;

    //! STL containers for DTED Cells.
    Dted_Cell_Set dted1CellSet;
    Dted_Cell_Set dted2CellSet;

    Dted_Cell_Path_Entry prevFailedCellPathEntry;

    bool bilinearInterpActive;

    // Previous cell iterator - attempt to use last found cell to avoid
    // researching through all cells
    Dted_Cell_Set::const_iterator prevCellItDted1;
    Dted_Cell_Set::const_iterator prevCellItDted2;

    //! Defines the access method.
    Access_Method accessMethod;

    //! Retrieve a Dted1 geolocation.
    double Get_Geo_Elev_Dted1(Geo_Location geoLoc);

    //! Retrieve a Dted1 post.
    double Get_Post_Elev_Dted1(Geo_Location geoLoc, Voxel pointLoc);

    //! Retrieve a Dted2 geolocation.
    double Get_Geo_Elev_Dted2(Geo_Location geoLoc);

    //! Retrieve a Dted2 post.
    double Get_Post_Elev_Dted2(Geo_Location geoLoc, Voxel pointLoc);

    bool debug;
};

#endif

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

#include "Dted_Database.h"
#include "Dted_Cell_Path_Entry.h"
#include "Dted_Cell.h"

Dted_Database::Dted_Database() {
    dted1CellSet.clear();
    dted2CellSet.clear();
    dted1_dir.Clear_Dted_Directory();
    dted2_dir.Clear_Dted_Directory();

    // Default to memory access
    accessMethod = MEMORY_ACCESS;
    dtedLevel = LEVEL_1;
    bilinearInterpActive = false;

    prevFailedCellPathEntry.latitude = -32767;
    prevFailedCellPathEntry.longitude = -32767;

    debug = false;
}

void Dted_Database::Clear_Database() {
    Dted_Cell_Set::const_iterator it = dted1CellSet.begin();

    while (it != dted1CellSet.end()) {
        Dted_Cell* dtedCellPtr = (Dted_Cell*) *it;
        delete dtedCellPtr;

        it++;
    }

    it = dted2CellSet.begin();

    while (it != dted2CellSet.end()) {
        Dted_Cell* dtedCellPtr = (Dted_Cell*) *it;
        delete dtedCellPtr;

        it++;
    }

    dted1CellSet.clear();
    dted2CellSet.clear();

    dted1_dir.Clear_Dted_Directory();
    dted2_dir.Clear_Dted_Directory();

    prevCellItDted1 = (Dted_Cell_Set::const_iterator) NULL;
    prevCellItDted2 = (Dted_Cell_Set::const_iterator) NULL;

    prevFailedCellPathEntry.latitude = -32767;
    prevFailedCellPathEntry.longitude = -32767;

}

void Dted_Database::Set_Bilinear_Interp_Active(bool newState) {
    bilinearInterpActive = newState;
}

double Dted_Database::Get_Geo_Elev(Geo_Location geoLoc) {
    double returnElev = 0.0;

    if (dtedLevel == LEVEL_1)
        returnElev = Get_Geo_Elev_Dted1(geoLoc);
    else if (dtedLevel == LEVEL_2)
        returnElev = Get_Geo_Elev_Dted2(geoLoc);

    return returnElev;
}

double Dted_Database::Get_Post_Elev(Geo_Location geoLoc, Voxel pointLoc) {
    double returnElev = 0.0;

    if (dtedLevel == LEVEL_1)
        returnElev = Get_Post_Elev_Dted1(geoLoc, pointLoc);
    else if (dtedLevel == LEVEL_2)
        returnElev = Get_Post_Elev_Dted2(geoLoc, pointLoc);

    return returnElev;
}

double Dted_Database::Get_Geo_Elev_Dted1(Geo_Location geoLoc) {
    bool found = false;

    double elevHeight = 0.0;
    Dted_Cell* dtedCellPtr = NULL;

    Dted_Cell_Set::const_iterator it;

    Dted_Cell_Path_Entry dtedCellPathEntry;

    dtedCellPathEntry.latitude = (short int) geoLoc.lat;

    if (geoLoc.lat < 0.0)
        dtedCellPathEntry.latitude -= 1;

    dtedCellPathEntry.longitude = (short int) geoLoc.lon;

    if (geoLoc.lon < 0)
        dtedCellPathEntry.longitude -= 1;

    if (dtedCellPathEntry == prevFailedCellPathEntry) {
        elevHeight = NULL_POST;
    } else   // Normal: not the same cell path as the previous failed position
    {
        // Attempt to use last found cell to avoid re-searching through all cells
        if (prevCellItDted1 != (Dted_Cell_Set::const_iterator) NULL) {
            it = prevCellItDted1;
            dtedCellPtr = (Dted_Cell*) *it;
            found = dtedCellPtr->covers(geoLoc);
        }

        it = dted1CellSet.begin();

        // Search for cell that provides coverage
        while ((it != dted1CellSet.end()) && (found == false)) {
            dtedCellPtr = (Dted_Cell*) *it;

            found = dtedCellPtr->covers(geoLoc);

            if (found)
                prevCellItDted1 = it;

            it++;
        }

        if (found) {
            dtedCellPtr->setBilinearInterpActive(bilinearInterpActive);

            if (accessMethod == MEMORY_ACCESS) {
                elevHeight = dtedCellPtr->getHeightAboveMSL(geoLoc);
            } else // Using disk access
            {
                elevHeight = dtedCellPtr->getHeightAboveMSLFromDisk(geoLoc);
            }
        } else {
            // Load cell from file.
            if (dted1_dir.Retrieve_Dted_Entry(dtedCellPathEntry)) {
                Dted_Cell* dtedCellPtr = new Dted_Cell(
                        dtedCellPathEntry.cellPath);

                if (accessMethod == MEMORY_ACCESS)
                    dtedCellPtr->loadCellFromDisk();

                dted1CellSet.insert(dtedCellPtr);

                // Call the method again.  This time the cell will be in the database.
                return Get_Geo_Elev_Dted1(geoLoc);
            } else {
                elevHeight = NULL_POST;
                prevFailedCellPathEntry = dtedCellPathEntry;
            }
        }
    }

    if (elevHeight == NULL_POST) {
        elevHeight = 0.0;
    }

    return elevHeight;
}

double Dted_Database::Get_Post_Elev_Dted1(Geo_Location geoLoc, Voxel pointLoc) {
    bool found = false;
    double elevHeight = 0.0;
    Dted_Cell* dtedCellPtr = NULL;

    // Memory Access
    Dted_Cell_Set::const_iterator it;

    Dted_Cell_Path_Entry dtedCellPathEntry;

    dtedCellPathEntry.latitude = (short int) geoLoc.lat;

    if (dtedCellPathEntry.latitude < 0)
        dtedCellPathEntry.latitude -= 1;

    dtedCellPathEntry.longitude = (short int) geoLoc.lon;

    if (dtedCellPathEntry.longitude < 0)
        dtedCellPathEntry.longitude -= 1;

    if (dtedCellPathEntry == prevFailedCellPathEntry) {
        elevHeight = NULL_POST;
    } else   // Normal: not the same cell path as the previous failed position
    {
        // Attempt to use last found cell to avoid re-searching through all cells
        if (prevCellItDted1 != Dted_Cell_Set::const_iterator(NULL)) {
            it = prevCellItDted1;
            dtedCellPtr = (Dted_Cell*) *it;
            found = dtedCellPtr->covers(geoLoc);
        }

        it = dted1CellSet.begin();

        // Search for cell that provides coverage
        while ((it != dted1CellSet.end()) && (found == false)) {
            dtedCellPtr = (Dted_Cell*) *it;

            found = dtedCellPtr->covers(geoLoc);

            if (found)
                prevCellItDted1 = it;

            it++;
        }

        if (found) {
            if (accessMethod == MEMORY_ACCESS)
                elevHeight = dtedCellPtr->getPostValue(pointLoc);
            else
                // Using disk access
                elevHeight = dtedCellPtr->getPostValueFromDisk(pointLoc);
        } else {
            // Load cell from file
            if (dted1_dir.Retrieve_Dted_Entry(dtedCellPathEntry)) {
                Dted_Cell* dtedCellPtr = new Dted_Cell(
                        dtedCellPathEntry.cellPath);

                if (accessMethod == MEMORY_ACCESS)
                    dtedCellPtr->loadCellFromDisk();

                dted1CellSet.insert(dtedCellPtr);

                // Call the method again.  This time the cell will be in the database.
                return Get_Post_Elev_Dted1(geoLoc, pointLoc);

            } else {
                elevHeight = NULL_POST;
                prevFailedCellPathEntry = dtedCellPathEntry;
            }
        }
    }

    if (elevHeight == NULL_POST) {
        elevHeight = 0.0;
    }

    return elevHeight;

}

double Dted_Database::Get_Geo_Elev_Dted2(Geo_Location geoLoc) {
    bool found = false;
    double elevHeight = 0.0;
    Dted_Cell* dtedCellPtr = NULL;

    // Memory Access
    Dted_Cell_Set::const_iterator it;

    Dted_Cell_Path_Entry dtedCellPathEntry;

    dtedCellPathEntry.latitude = (short int) geoLoc.lat;

    if (dtedCellPathEntry.latitude < 0)
        dtedCellPathEntry.latitude -= 1;

    dtedCellPathEntry.longitude = (short int) geoLoc.lon;

    if (dtedCellPathEntry.longitude < 0)
        dtedCellPathEntry.longitude -= 1;

    if (dtedCellPathEntry == prevFailedCellPathEntry) {
        elevHeight = NULL_POST;
    } else   // Normal: not the same cell path as the previous failed position
    {

        // Attempt to use last found cell to avoid re-searching through all cells
        if (prevCellItDted2 != (Dted_Cell_Set::const_iterator) NULL) {
            it = prevCellItDted2;
            dtedCellPtr = (Dted_Cell*) *it;
            found = dtedCellPtr->covers(geoLoc);
        }

        it = dted2CellSet.begin();

        // Search for cell that provides coverage within database
        while ((it != dted2CellSet.end()) && (found == false)) {
            dtedCellPtr = (Dted_Cell*) *it;

            found = dtedCellPtr->covers(geoLoc);

            if (found)
                prevCellItDted2 = it;

            it++;
        }

        if (found) {
            dtedCellPtr->setBilinearInterpActive(bilinearInterpActive);

            if (accessMethod == MEMORY_ACCESS)
                elevHeight = dtedCellPtr->getHeightAboveMSL(geoLoc);
            else
                // Using disk access
                elevHeight = dtedCellPtr->getHeightAboveMSLFromDisk(geoLoc);
        } else {
            // Load cell from file
            if (dted2_dir.Retrieve_Dted_Entry(dtedCellPathEntry)) {
                Dted_Cell* dtedCellPtr = new Dted_Cell(
                        dtedCellPathEntry.cellPath);

                if (accessMethod == MEMORY_ACCESS)
                    dtedCellPtr->loadCellFromDisk();

                dted2CellSet.insert(dtedCellPtr);

                // Call the method again.  This time the cell will be in the database.
                return Get_Geo_Elev_Dted2(geoLoc);
            } else {
                elevHeight = NULL_POST;
                prevFailedCellPathEntry = dtedCellPathEntry;
            }
        }

    }   // prevFailedCellPathEntry

    if (elevHeight == NULL_POST) {
        elevHeight = 0.0;
    }

    return elevHeight;

}

double Dted_Database::Get_Post_Elev_Dted2(Geo_Location geoLoc, Voxel pointLoc) {
    bool found = false;
    double elevHeight = 0.0;
    Dted_Cell* dtedCellPtr = NULL;

    // Memory Access
    Dted_Cell_Set::const_iterator it;

    Dted_Cell_Path_Entry dtedCellPathEntry;

    dtedCellPathEntry.latitude = (short int) geoLoc.lat;

    if (dtedCellPathEntry.latitude < 0)
        dtedCellPathEntry.latitude -= 1;

    dtedCellPathEntry.longitude = (short int) geoLoc.lon;

    if (dtedCellPathEntry.longitude < 0)
        dtedCellPathEntry.longitude -= 1;

    if (dtedCellPathEntry == prevFailedCellPathEntry) {
        elevHeight = NULL_POST;
    } else   // Normal: not the same cell path as the previous failed position
    {

        // Attempt to use last found cell to avoid re-searching through all cells
        if (prevCellItDted2 != (Dted_Cell_Set::const_iterator) NULL) {
            it = prevCellItDted2;
            dtedCellPtr = (Dted_Cell*) *it;
            found = dtedCellPtr->covers(geoLoc);
        }

        it = dted2CellSet.begin();

        // Search for cell that provides coverage
        while ((it != dted2CellSet.end()) && (found == false)) {
            dtedCellPtr = (Dted_Cell*) *it;

            found = dtedCellPtr->covers(geoLoc);

            if (found)
                prevCellItDted2 = it;

            it++;
        }

        if (found) {
            if (accessMethod == MEMORY_ACCESS)
                elevHeight = dtedCellPtr->getPostValue(pointLoc);
            else
                // Using disk access
                elevHeight = dtedCellPtr->getPostValueFromDisk(pointLoc);
        } else {
            // Load cell from file
            if (dted2_dir.Retrieve_Dted_Entry(dtedCellPathEntry)) {
                Dted_Cell* dtedCellPtr = new Dted_Cell(
                        dtedCellPathEntry.cellPath);

                if (accessMethod == MEMORY_ACCESS)
                    dtedCellPtr->loadCellFromDisk();

                dted2CellSet.insert(dtedCellPtr);

                // Call the method again.  This time the cell will be in the database.
                return Get_Post_Elev_Dted2(geoLoc, pointLoc);
            } else {
                elevHeight = NULL_POST;
                prevFailedCellPathEntry = dtedCellPathEntry;
            }
        }
    }

    if (elevHeight == NULL_POST) {
        elevHeight = 0.0;
    }

    return elevHeight;
}

void Dted_Database::Set_Dted_Level(Dted_Level newDtedLevel) {
    dtedLevel = newDtedLevel;
}

Dted_Level Dted_Database::Get_Dted_Level() {
    return dtedLevel;
}

bool Dted_Database::Populate_Dted1_Directory(const string& path, bool preLoad) {
    Dted_Cell_Path_Entry dtedCellPathEntry;

    dted1_dir.Populate_Directory(path);

    if (preLoad) {
        dted1_dir.QueryReset();

        while (dted1_dir.Query(dtedCellPathEntry)) {
            Dted_Cell* dtedCellPtr = new Dted_Cell(dtedCellPathEntry.cellPath);

            dtedCellPtr->loadCellFromDisk();
            dted1CellSet.insert(dtedCellPtr);
        }
    }

    if (debug) {
        cout << "\nDL1 Minimum meridian is : " << dted1_dir.getMinMeridian()
                << endl << "DL1 Maximum meridian is   : "
                << dted1_dir.getMaxMeridian() << endl
                << "DL1 Minimum parallel is   : " << dted1_dir.getMinParallel()
                << endl << "DL1 Maximum parallel is   : "
                << dted1_dir.getMaxParallel() << endl;
    }

    return true;

}

bool Dted_Database::Populate_Dted2_Directory(const string& path, bool preLoad) {
    Dted_Cell_Path_Entry dtedCellPathEntry;

    dted2_dir.Populate_Directory(path);

    if (preLoad) {
        dted2_dir.QueryReset();

        while (dted2_dir.Query(dtedCellPathEntry)) {
            Dted_Cell* dtedCellPtr = new Dted_Cell(dtedCellPathEntry.cellPath);

            dtedCellPtr->loadCellFromDisk();
            dted2CellSet.insert(dtedCellPtr);
        }
    }

    if (debug) {
        cout << "\nDL2 Minimum meridian is : " << dted2_dir.getMinMeridian()
                << endl << "DL2 Maximum meridian is   : "
                << dted2_dir.getMaxMeridian() << endl
                << "DL2 Minimum parallel is   : " << dted2_dir.getMinParallel()
                << endl << "DL2 Maximum parallel is   : "
                << dted2_dir.getMaxParallel() << endl;
    }

    return true;
}

void Dted_Database::Gather_Stats_Dted1() {
    float minHeightAboveMSL = 65536;
    float maxHeightAboveMSL = 0;

    Dted_Cell_Set::const_iterator it = dted1CellSet.begin();

    while (it != dted1CellSet.end()) {
        Dted_Cell* dtedCellPtr = (Dted_Cell*) *it;

        dtedCellPtr->gatherStatistics();

        if (dtedCellPtr->minHeightAboveMSL() < minHeightAboveMSL)
            minHeightAboveMSL = dtedCellPtr->minHeightAboveMSL();

        if (dtedCellPtr->maxHeightAboveMSL() > maxHeightAboveMSL)
            maxHeightAboveMSL = dtedCellPtr->maxHeightAboveMSL();

        it++;
    }

    cout << "Stats Report" << endl << "min DTED1 = " << minHeightAboveMSL
            << endl << "max DTED1 = " << maxHeightAboveMSL << endl;

}

void Dted_Database::Gather_Stats_Dted2() {
    float minHeightAboveMSL = 65536;
    float maxHeightAboveMSL = 0;

    Dted_Cell_Set::const_iterator it = dted2CellSet.begin();

    while (it != dted2CellSet.end()) {
        Dted_Cell* dtedCellPtr = (Dted_Cell*) *it;

        dtedCellPtr->gatherStatistics();

        if (dtedCellPtr->minHeightAboveMSL() < minHeightAboveMSL)
            minHeightAboveMSL = dtedCellPtr->minHeightAboveMSL();

        if (dtedCellPtr->maxHeightAboveMSL() > maxHeightAboveMSL)
            maxHeightAboveMSL = dtedCellPtr->maxHeightAboveMSL();

        it++;
    }

    cout << "Stats Report" << endl << "min DTED2 = " << minHeightAboveMSL
            << endl << "max DTED2 = " << maxHeightAboveMSL << endl;

}

void Dted_Database::Set_Access_Method(Access_Method newMethod) {
    accessMethod = newMethod;
}

Access_Method Dted_Database::Get_Access_Method() const {
    return accessMethod;
}


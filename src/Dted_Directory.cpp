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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "Dted_Directory.h"

#if defined(WIN32)
#include "posixwin.h"
#endif

Dted_Directory::Dted_Directory() :
        minMeridian("E180"),
        maxMeridian("W180"),
        minParallel("N90"),
        maxParallel("S90") {
    debug = false;
}

bool Dted_Directory::Populate_Directory(const string& pathName) {
    // Open the directory.

    DIR *The_Directory = opendir(pathName.c_str());

    if (NULL == The_Directory) {
        cout << "WARNING> Can not find the DTED directory: '" << pathName
                << "' (no data for this DTED Level ?)." << endl;

        return false;
    }

    struct dirent *Directory_Entry = readdir(The_Directory);
    bool Found = false;
    string Search_Name(pathName.c_str());

    while (NULL != Directory_Entry) {
        string File_Name("/");
        File_Name.append(Directory_Entry->d_name);

        Search_Name += File_Name;

        // Report in, if requested.

        if (debug)
            cout << "...searching " << Search_Name << endl << flush;

        // Check on this entry.

        struct stat Directory_Stat;
        lstat(Search_Name.c_str(), &Directory_Stat);
        const bool Is_Directory = S_ISDIR (Directory_Stat.st_mode);

        // If this is a file...

        if (!Is_Directory) {
            if (Is_Dted_File(Directory_Entry->d_name)) {
                // Add to Path Entry set
                Dted_Cell_Path_Entry path_Entry;

                path_Entry.longitude = currPathMeridian;

                string parallelTxt = Directory_Entry->d_name;
                parallelTxt = parallelTxt.substr(0,
                        parallelTxt.find_first_of("."));

                // Find MIN Parallel (-90.0 <= Latitude <= +90.0)
                if ((parallelTxt.compare(0, 1, "S") == 0)
                        || (parallelTxt.compare(0, 1, "s") == 0)) {
                    if ((minParallel.compare(0, 1, "N") == 0)
                            || (minParallel.compare(0, 1, "n") == 0)) {
                        minParallel = parallelTxt;
                    } else {
                        if (parallelTxt.compare(1, 2, minParallel, 1, 2) > 0) {
                            minParallel = parallelTxt;
                        }
                    }
                } else {
                    if ((minParallel.compare(0, 1, "N") == 0)
                            || (minParallel.compare(0, 1, "n") == 0)) {
                        if (parallelTxt.compare(1, 2, minParallel, 1, 2) < 0) {
                            minParallel = parallelTxt;
                        }
                    }
                }

                // Find max parallel (-90.0 <= latitude <= +90.0)
                if ((parallelTxt.compare(0, 1, "N") == 0)
                        || (parallelTxt.compare(0, 1, "n") == 0)) {
                    if ((maxParallel.compare(0, 1, "S") == 0)
                            || (maxParallel.compare(0, 1, "s") == 0)) {
                        maxParallel = parallelTxt;
                    } else {
                        if (parallelTxt.compare(1, 2, maxParallel, 1, 2) > 0) {
                            maxParallel = parallelTxt;
                        }
                    }
                } else {
                    if ((maxParallel.compare(0, 1, "S") == 0)
                            || (maxParallel.compare(0, 1, "s") == 0)) {
                        if (parallelTxt.compare(1, 2, maxParallel, 1, 2) < 0) {
                            maxParallel = parallelTxt;
                        }
                    }
                }

                string latitudeTxt = "";

                unsigned int n = 1;
                while ((Directory_Entry->d_name[n] != '.')
                        && (n < sizeof(Directory_Entry->d_name))) {
                    latitudeTxt += Directory_Entry->d_name[n];
                    n++;
                }

                short latitude = atoi(latitudeTxt.c_str());

                if (('S' == Directory_Entry->d_name[0])
                        || ('s' == Directory_Entry->d_name[0]))
                    latitude = latitude * (-1);

                path_Entry.latitude = latitude;
                path_Entry.cellPath = Search_Name;

                if (debug) {
                    cout << "Added DTED cell:" << endl;
                    cout << "Latitude = " << path_Entry.latitude << endl;
                    cout << "Longitude = " << path_Entry.longitude << endl;
                    cout << "Path = " << path_Entry.cellPath << endl << endl;
                }

                pathEntrySet.insert(path_Entry);
            }
        }

        // else this is a directory.  Ignore directory "." and ".." entries,
        // and continue looking...
        else if ((0 != strcmp(Directory_Entry->d_name, "."))
                && (0 != strcmp(Directory_Entry->d_name, ".."))) {
            // Determine if this is a dted meridian directory
            if (('E' == Directory_Entry->d_name[0])
                    || ('e' == Directory_Entry->d_name[0])
                    || ('W' == Directory_Entry->d_name[0])
                    || ('w' == Directory_Entry->d_name[0])) {
                string meridianTxt = Directory_Entry->d_name;

                if ((meridianTxt.compare(0, 1, "W") == 0)
                        || (meridianTxt.compare(0, 1, "w") == 0)) {
                    if ((minMeridian.compare(0, 1, "E") == 0)
                            || (minMeridian.compare(0, 1, "e") == 0)) {
                        minMeridian = meridianTxt;
                    } else {
                        if (meridianTxt.compare(1, 3, minMeridian, 1, 3) > 0) {
                            minMeridian = meridianTxt;
                        }
                    }
                } else {
                    if ((minMeridian.compare(0, 1, "E") == 0)
                            || (minMeridian.compare(0, 1, "e") == 0)) {
                        if (meridianTxt.compare(1, 3, minMeridian, 1, 3) < 0) {
                            minMeridian = meridianTxt;
                        }
                    }
                }

                if ((meridianTxt.compare(0, 1, "E") == 0)
                        || (meridianTxt.compare(0, 1, "e") == 0)) {
                    if ((maxMeridian.compare(0, 1, "W") == 0)
                            || (maxMeridian.compare(0, 1, "w") == 0)) {
                        maxMeridian = meridianTxt;
                    } else {
                        if (meridianTxt.compare(1, 3, maxMeridian, 1, 3) > 0) {
                            maxMeridian = meridianTxt;
                        }
                    }
                } else {
                    if ((maxMeridian.compare(0, 1, "W") == 0)
                            || (maxMeridian.compare(0, 1, "w") == 0)) {
                        if (meridianTxt.compare(1, 3, maxMeridian, 1, 3) < 0) {
                            maxMeridian = meridianTxt;
                        }
                    }
                }

                // Process the Meridian sign difference (West is negative)
                meridianTxt = meridianTxt.substr(1, (meridianTxt.length() - 1));
                currPathMeridian = atoi(meridianTxt.c_str());

                if (('W' == Directory_Entry->d_name[0])
                        || ('w' == Directory_Entry->d_name[0])) {
                    currPathMeridian = currPathMeridian * (-1);
                }

                if (debug)
                    cout << "Current meridian directory = " << currPathMeridian
                            << endl;
            }

            // Search recursively through the directory path names until the
            // end is reached.
            Found = this->Populate_Directory(Search_Name.c_str());
        }

        // Remove the entry name from the search name.
        Search_Name = Search_Name.substr(0, Search_Name.rfind(File_Name));

        // Get the next directory entry.
        Directory_Entry = readdir(The_Directory);
    }

    // Close the directory.
    if (-1 == closedir(The_Directory)) {
        perror("Dted_Directory::Search.closedir");
    }

    // Return the status of the search.
    return Found;
}

bool Dted_Directory::Retrieve_Dted_Entry(
        Dted_Cell_Path_Entry &dted_Cell_Path_Entry) {
    Path_Entry_Set::const_iterator it = pathEntrySet.begin();

    while (it != pathEntrySet.end()) {
        if (*it == dted_Cell_Path_Entry) {
            dted_Cell_Path_Entry = *it;
            return true;
        }
        it++;
    }

    return false;
}

void Dted_Directory::Clear_Dted_Directory() {
    // Reinit (min, max) of map (Long, Lat) for new database

    minMeridian = "E180";
    maxMeridian = "W180";
    minParallel = "N90";
    maxParallel = "S90";

    pathEntrySet.clear();
}

void Dted_Directory::Dump_Path_Entry_Set() {
    Path_Entry_Set::const_iterator it = pathEntrySet.begin();

    while (it != pathEntrySet.end()) {
        cout << "Latitude = " << it->latitude << endl;
        cout << "Longitude = " << it->longitude << endl;
        cout << "Path = " << it->cellPath << endl << endl;
        it++;
    }
}

bool Dted_Directory::Is_Dted_File(string fileName) {
    // Erase the filename up to the last '.' to obtain extension
    fileName.erase(0, fileName.find_last_of('.') + 1);

    // Compare for all levels of DTED 0 - 2
    if ((fileName.compare("DT0") == 0) || (fileName.compare("Dt0") == 0)
            || (fileName.compare("dT0") == 0) || (fileName.compare("dt0") == 0)
            || (fileName.compare("DT1") == 0) || (fileName.compare("Dt1") == 0)
            || (fileName.compare("dT1") == 0) || (fileName.compare("dt1") == 0)
            || (fileName.compare("DT2") == 0) || (fileName.compare("Dt2") == 0)
            || (fileName.compare("dT2") == 0) || (fileName.compare("dt2") == 0))
        return true;
    else
        return false;
}

void Dted_Directory::QueryReset(void) {
    querytIt = pathEntrySet.begin();
}

bool Dted_Directory::Query(Dted_Cell_Path_Entry &dtedCellPathEntry) {
    if (querytIt == pathEntrySet.end())
        return false;

    dtedCellPathEntry = *querytIt;

    querytIt++;

    return true;
}

string Dted_Directory::getMinMeridian() {
    return minMeridian;
}

string Dted_Directory::getMaxMeridian() {
    return maxMeridian;
}

string Dted_Directory::getMinParallel() {
    return minParallel;
}

string Dted_Directory::getMaxParallel() {
    return maxParallel;
}

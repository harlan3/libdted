//*****************************************************************************
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//
// DESCRIPTION:
//   Contains constants and data structures used throughout Dted library.
//
//*****************************************************************************

#ifndef Dted_Common
#define Dted_Common

#include <float.h>

const double DBL_NAN = -1.0 / DBL_EPSILON;
const int INT_NAN = 0x80000000;
const short NULL_POST = -32767; // Fixed by DTED specification.

//! Architecture endian type.
enum ByteOrder {
    ARCH_LITTLE_ENDIAN = 0, ARCH_BIG_ENDIAN
};

//! Access method
enum Access_Method {
    MEMORY_ACCESS = 0, DISK_ACCESS
};

//! Defines the DTED_Level for a cell.
typedef enum {
    LEVEL_0, LEVEL_1, LEVEL_2,
} Dted_Level;

//! Defines the various interpolation modes
typedef enum {
    BILINEAR, NEAREST_NEIGHBOR
} Interpolation_Mode;

//! Defines the Cell_Size structure.
typedef struct {
    //! The number of lonLines for cell.
    int lonLines;
    //! The number of latPoints for cell.
    int latPoints;
} Cell_Size;

//! Defines the Geo_Location structure.
typedef struct {
    //! The lat component of the geo location.
    double lat;
    //! The lon component of the geo location.
    double lon;
} Geo_Location;

//! Defines the Voxel structure.
typedef struct {
    //! The x component of the voxel.
    double x;
    //! The y component of the voxel.
    double y;
    //! The z component of the voxel.
    double z;
} Voxel;

#endif

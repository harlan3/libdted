//******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description:  This class provides byte swap routines and access to the
//               endian type for architecture.
//
//***********************************

#ifndef Endian_HEADER
#define Endian_HEADER

#include "Dted_Common.h"
#include <iostream>
using namespace std;

//  Purpose:
//! This class provides byte swap routines and access to the endian
//! type for architecture.

class Endian {
public:

    //! Endian constructor
    Endian();

    //! Perform byteswap on short.
    void swap(short &data) const;

    //! Perform byteswap on unsigned short.
    void swap(unsigned short &data) const;

    //! Perform byteswap on int.
    void swap(int &data) const;

    //! Perform byteswap on unsigned int.
    void swap(unsigned int &data) const;

    //! Perform byteswap on float.
    void swap(float &data) const;

    //! Perform byteswap on double.
    void swap(double &data) const;

    //! Perform byteswap on short.
    void swap(short* data, unsigned int size) const;

    //! Perform byteswap on unsigned short.
    void swap(unsigned short* data, unsigned int size) const;

    //! Perform byteswap on int*.
    void swap(int* data, unsigned int size) const;

    //! Perform byteswap on unsigned int*.
    void swap(unsigned int* data, unsigned int size) const;

    //! Perform byteswap on long*.
    void swap(long* data, unsigned int size) const;

    //! Perform byteswap on unsigned long*.
    void swap(unsigned long* data, unsigned int size) const;

    //! Perform byteswap on float*.
    void swap(float* data, unsigned int size) const;

    //! Perform byteswap on double*.
    void swap(double* data, unsigned int size) const;

    //! Perform byteswap on two bytes.
    void swapTwoBytes(void* data, unsigned int size) const;

    //! Perform byteswap on four bytes.
    void swapFourBytes(void* data, unsigned int size) const;

    //! Perform byteswap on eight bytes.
    void swapEightBytes(void* data, unsigned int size) const;

    //! Return architecture endian type.
    ByteOrder getSystemEndianType() const;

private:

    // Holds the Endian of the target architecture
    ByteOrder theSystemEndianType;

    void swapTwoBytes(void *data) const;
    void swapFourBytes(void *data) const;
    void swapEightBytes(void *data) const;

    void swap(unsigned char *c1, unsigned char *c2) const;
};

#endif

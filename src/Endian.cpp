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

#include "Endian.h"

Endian::Endian() {
    unsigned short test;
    unsigned char *testPtr = NULL;
    test = 0x0001;

    testPtr = reinterpret_cast<unsigned char*>(&test);
    theSystemEndianType = testPtr[0] ? ARCH_LITTLE_ENDIAN : ARCH_BIG_ENDIAN;
}

ByteOrder Endian::getSystemEndianType() const {
    return theSystemEndianType;
}

void Endian::swap(short &data) const {
    swapTwoBytes(reinterpret_cast<void*>(&data));
}

void Endian::swap(unsigned short &data) const {
    swapTwoBytes(reinterpret_cast<void*>(&data));
}

void Endian::swap(int &data) const {
    swapFourBytes(reinterpret_cast<void*>(&data));
}

void Endian::swap(unsigned int &data) const {
    swapFourBytes(reinterpret_cast<void*>(&data));
}

void Endian::swap(float &data) const {
    swapFourBytes(reinterpret_cast<void*>(&data));
}

void Endian::swap(double &data) const {
    swapEightBytes(reinterpret_cast<void*>(&data));
}

void Endian::swapTwoBytes(void *data) const {
    unsigned char *c = reinterpret_cast<unsigned char*>(data);

    swap(&c[0], &c[1]);
}

void Endian::swapFourBytes(void* data) const {
    unsigned char *c = reinterpret_cast<unsigned char*>(data);

    swap(&c[0], &c[3]);
    swap(&c[1], &c[2]);
}

void Endian::swapEightBytes(void* data) const {
    unsigned char *c = reinterpret_cast<unsigned char*>(data);

    swap(&c[0], &c[7]);
    swap(&c[1], &c[6]);
    swap(&c[2], &c[5]);
    swap(&c[3], &c[4]);
}

void Endian::swap(unsigned char *c1, unsigned char *c2) const {
    unsigned char temp_c = *c1;
    *c1 = *c2;
    *c2 = temp_c;
}

void Endian::swap(short* data, unsigned int size) const {
    swapTwoBytes(data, size);
}

void Endian::swap(unsigned short* data, unsigned int size) const {
    swapTwoBytes(data, size);
}

void Endian::swap(int* data, unsigned int size) const {
    swapFourBytes(data, size);
}

void Endian::swap(unsigned int* data, unsigned int size) const {
    swapFourBytes(data, size);
}

void Endian::swap(long* data, unsigned int size) const {
    swapEightBytes(data, size);
}

void Endian::swap(unsigned long* data, unsigned int size) const {
    swapEightBytes(data, size);
}

void Endian::swap(float* data, unsigned int size) const {
    swapFourBytes(data, size);
}

void Endian::swap(double* data, unsigned int size) const {
    swapEightBytes(data, size);
}

void Endian::swapTwoBytes(void* data, unsigned int size) const {
    unsigned short* buf = reinterpret_cast<unsigned short*>(data);
    for (unsigned int i = 0; i < size; ++i) {
        buf[i] = (buf[i] << 8) | (buf[i] >> 8);
    }
}

void Endian::swapFourBytes(void* data, unsigned int size) const {
    unsigned int* buf = reinterpret_cast<unsigned int*>(data);
    for (unsigned int i = 0; i < size; ++i) {
        buf[i] = (((buf[i] & 0xff000000) >> 24) | ((buf[i] & 0x00ff0000) >> 8)
                | ((buf[i] & 0x0000ff00) << 8) | ((buf[i] & 0x000000ff) << 24));
    }
}

void Endian::swapEightBytes(void* data, unsigned int size) const {
#ifdef HAS_LONG_LONG
    uint64* buf = reinterpret_cast<unsigned long*>(data);
    for (uint32 i=0; i<size; ++i)
    {
        buf[i]
              = ( ((buf[i] & 0xff00000000000000ull) >> 56)
                | ((buf[i] & 0x00ff000000000000ull) >> 40)
                | ((buf[i] & 0x0000ff0000000000ull) >> 24)
                | ((buf[i] & 0x000000ff00000000ull) >> 8)
                | ((buf[i] & 0x00000000ff000000ull) << 8)
                | ((buf[i] & 0x0000000000ff0000ull) << 24)
                | ((buf[i] & 0x000000000000ff00ull) << 40)
                | ((buf[i] & 0x00000000000000ffull) << 56));
    }
#else
    double* buf = reinterpret_cast<double*>(data);
    for (unsigned int i = 0; i < size; ++i) {
        swapEightBytes(buf + i);
    }

#endif

}

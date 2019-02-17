// Copyright 2019 nakat-t <armaiti.wizard@gmail.com>
// This file is subject to the license terms in the LICENSE file 
// found in the top-level directory of this distribution.

// Please define ENDIAN_IS_LITTLE or ENDIAN_IS_BIG macro before include "endian.h"
#define ENDIAN_IS_LITTLE
//#define ENDIAN_IS_BIG
// ..OR, define the ENDIAN_DETECT macro to detect endianness at run time.
// In this case there is a penalty for the execution speed.
//#define ENDIAN_DETECT

#include "endian.h"
#include <cstdio>
#include <cassert>

int main()
{
	if(endian::is_little()) {
		printf("little endian\n");
	}
	else {
		printf("big endian\n");
	}

	assert(endian::bswap16(uint16_t{0x0102}) == uint16_t{0x0201});
	assert(endian::bswap32(uint32_t{0x01020304}) == uint32_t{0x04030201});
	assert(endian::bswap64(uint64_t{0x0102030405060708}) == uint64_t{0x0807060504030201});

	union {
		uint16_t i;
		uint8_t c[sizeof(uint16_t)];
	} x16 = {0x0102};

	x16.i = endian::hton16(x16.i);
	assert(x16.c[0] == 1 && x16.c[1] == 2);
	x16.i = endian::ntoh16(x16.i);

	assert(x16.i == uint16_t{0x0102});
	if(endian::is_little()) {
		assert(x16.c[0] == 2 && x16.c[1] == 1);
	}
	else {
		assert(x16.c[0] == 1 && x16.c[1] == 2);
	}
}

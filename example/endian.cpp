// Copyright 2019 nakat-t <armaiti.wizard@gmail.com>
// This file is subject to the license terms in the LICENSE file 
// found in the top-level directory of this distribution.

// Please define ENDIAN_IS_LITTLE or ENDIAN_IS_BIG macro before include "endian.h"
#define ENDIAN_IS_LITTLE
//#define ENDIAN_IS_BIG
// ..OR, define the ENDIAN_DETECT macro to detect endianness at run time.
// In this case there is a penalty for the execution speed.
//#define ENDIAN_DETECT

#include "endian/endian.h"
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
	} x;

	x.i = endian::hton16(uint16_t{0x0102});
	assert(x.c[0] == 1 && x.c[1] == 2);

	x.i = endian::ntoh16(x.i);
	assert(x.i == uint16_t{0x0102});
	if(endian::is_little()) {
		assert(x.c[0] == 2 && x.c[1] == 1);
	}
	else {
		assert(x.c[0] == 1 && x.c[1] == 2);
	}

	x.i = endian::big::from_host16(uint16_t{0x0304});
	assert(x.c[0] == 3 && x.c[1] == 4);
	x.i = endian::big::to_host16(x.i);
	assert(x.i == uint16_t{0x0304});

	x.i = endian::little::from_host16(uint16_t{0x0304});
	assert(x.c[0] == 4 && x.c[1] == 3);
	x.i = endian::little::to_host16(x.i);
	assert(x.i == uint16_t{0x0304});

	return 0;
}

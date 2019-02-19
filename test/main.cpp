// Copyright 2019 nakat-t <armaiti.wizard@gmail.com>
// This file is subject to the license terms in the LICENSE file 
// found in the top-level directory of this distribution.

// Define `ENDIAN_IS_LITTLE` or `ENDIAN_IS_BIG` macro before include "endian.h".
// Alternatively, if you define the `ENDIAN_DETECT` macro, it detects endianness
// at run time. In that case, the speed will be slow.
#define ENDIAN_IS_LITTLE
//#define ENDIAN_IS_BIG
//#define ENDIAN_DETECT
#include <endian/endian.h>

#include <cassert>
#include <cstdint>

template<typename T>
union uc {
    T i;
    uint8_t c[sizeof(T)];
};

int main()
{
    { // Detection of endian.
#if defined(ENDIAN_IS_LITTLE)
        assert(endian::is_little());
        assert(!endian::is_big());
#elif defined(ENDIAN_IS_BIG)
        assert(!endian::is_little());
        assert(endian::is_big());
#endif
    }
    { // bswap() should exchange endian.
        assert(uint16_t{0x0102} == endian::bswap16(uint16_t{0x0201}));
        assert(uint32_t{0x01020304} == endian::bswap32(uint32_t{0x04030201}));
        assert(uint64_t{0x0102030405060708} == endian::bswap64(uint64_t{0x0807060504030201}));
        assert(uint8_t{0x01} == endian::bswap<uint8_t>(uint8_t{0x01}));
        assert(uint16_t{0x0102} == endian::bswap<uint16_t>(uint16_t{0x0201}));
        assert(uint32_t{0x01020304} == endian::bswap<uint32_t>(uint32_t{0x04030201}));
        assert(uint64_t{0x0102030405060708} == endian::bswap<uint64_t>(uint64_t{0x0807060504030201}));
    }
    { // hton8(),ntoh8() does nothing.
        assert(endian::hton<uint8_t>(uint8_t{0x01}) == uint8_t{0x01});
        assert(endian::ntoh<uint8_t>(uint8_t{0x01}) == uint8_t{0x01});
    }
    { // hton16(),ntoh16() should convert between host byte order and network byte order.
        uc<uint16_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = i+1;
        assert(endian::hton16(uint16_t{0x0102}) == x.i);
        assert(endian::hton<uint16_t>(uint16_t{0x0102}) == x.i);
        assert(endian::ntoh16(x.i) == uint16_t{0x0102});
        assert(endian::ntoh<uint16_t>(x.i) == uint16_t{0x0102});
    }
    { // hton32(),ntoh32() should convert between host byte order and network byte order.
        uc<uint32_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = i+1;
        assert(endian::hton32(uint32_t{0x01020304}) == x.i);
        assert(endian::hton<uint32_t>(uint32_t{0x01020304}) == x.i);
        assert(endian::ntoh32(x.i) == uint32_t{0x01020304});
        assert(endian::ntoh<uint32_t>(x.i) == uint32_t{0x01020304});
    }
    { // hton64(),ntoh64() should convert between host byte order and network byte order.
        uc<uint64_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = i+1;
        assert(endian::hton64(uint64_t{0x0102030405060708}) == x.i);
        assert(endian::hton<uint64_t>(uint64_t{0x0102030405060708}) == x.i);
        assert(endian::ntoh64(x.i) == uint64_t{0x0102030405060708});
        assert(endian::ntoh<uint64_t>(x.i) == uint64_t{0x0102030405060708});
    }
    { // big::to_host8(),from_host8() does nothing.
        assert(endian::big::from_host<uint8_t>(uint8_t{0x01}) == uint8_t{0x01});
        assert(endian::big::to_host<uint8_t>(uint8_t{0x01}) == uint8_t{0x01});
    }
    { // big::to_host16(),from_host16() converts between big endian and host endian.
        uc<uint16_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = i+1;
        assert(endian::big::from_host16(uint16_t{0x0102}) == x.i);
        assert(endian::big::from_host<uint16_t>(uint16_t{0x0102}) == x.i);
        assert(endian::big::to_host16(x.i) == uint16_t{0x0102});
        assert(endian::big::to_host<uint16_t>(x.i) == uint16_t{0x0102});
    }
    { // big::to_host32(),from_host32() converts between big endian and host endian.
        uc<uint32_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = i+1;
        assert(endian::big::from_host32(uint32_t{0x01020304}) == x.i);
        assert(endian::big::from_host<uint32_t>(uint32_t{0x01020304}) == x.i);
        assert(endian::big::to_host32(x.i) == uint32_t{0x01020304});
        assert(endian::big::to_host<uint32_t>(x.i) == uint32_t{0x01020304});
    }
    { // big::to_host64(),from_host64() converts between big endian and host endian.
        uc<uint64_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = i+1;
        assert(endian::big::from_host64(uint64_t{0x0102030405060708}) == x.i);
        assert(endian::big::from_host<uint64_t>(uint64_t{0x0102030405060708}) == x.i);
        assert(endian::big::to_host64(x.i) == uint64_t{0x0102030405060708});
        assert(endian::big::to_host<uint64_t>(x.i) == uint64_t{0x0102030405060708});
    }
    { // little::to_host8(),from_host8() does nothing.
        assert(endian::little::from_host<uint8_t>(uint8_t{0x01}) == uint8_t{0x01});
        assert(endian::little::to_host<uint8_t>(uint8_t{0x01}) == uint8_t{0x01});
    }
    { // little::to_host16(),from_host16() converts between little endian and host endian.
        uc<uint16_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = sizeof(x)-i;
        assert(endian::little::from_host16(uint16_t{0x0102}) == x.i);
        assert(endian::little::from_host<uint16_t>(uint16_t{0x0102}) == x.i);
        assert(endian::little::to_host16(x.i) == uint16_t{0x0102});
        assert(endian::little::to_host<uint16_t>(x.i) == uint16_t{0x0102});
    }
    { // little::to_host32(),from_host32() converts between big endian and host endian.
        uc<uint32_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = sizeof(x)-i;
        assert(endian::little::from_host32(uint32_t{0x01020304}) == x.i);
        assert(endian::little::from_host<uint32_t>(uint32_t{0x01020304}) == x.i);
        assert(endian::little::to_host32(x.i) == uint32_t{0x01020304});
        assert(endian::little::to_host<uint32_t>(x.i) == uint32_t{0x01020304});
    }
    { // little::to_host64(),from_host64() converts between big endian and host endian.
        uc<uint64_t> x;
        for(int i=0; i<sizeof(x); ++i) x.c[i] = sizeof(x)-i;
        assert(endian::little::from_host64(uint64_t{0x0102030405060708}) == x.i);
        assert(endian::little::from_host<uint64_t>(uint64_t{0x0102030405060708}) == x.i);
        assert(endian::little::to_host64(x.i) == uint64_t{0x0102030405060708});
        assert(endian::little::to_host<uint64_t>(x.i) == uint64_t{0x0102030405060708});
    }
    return 0;
}

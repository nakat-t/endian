// Copyright 2019 nakat-t <armaiti.wizard@gmail.com>
// This file is subject to the license terms in the LICENSE file 
// found in the top-level directory of this distribution.

#ifndef NAKATT_ENDIAN_H_
#define NAKATT_ENDIAN_H_

#include <cstdint>

namespace endian {

inline bool is_big()
{
#if defined(ENDIAN_IS_LITTLE)
	return false;
#elif defined(ENDIAN_IS_BIG)
	return true;
#else
	union {
		uint32_t i;
		uint8_t c[sizeof(uint32_t)];
	} x = {0x01020304};
	return x.c[0] == 0x01;
#endif
}

inline bool is_little()
{
	return !is_big();
}

#if defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 8) || __GNUC__ >= 5))
inline uint16_t bswap16(uint16_t n)
{
	return __builtin_bswap16(n);
}
inline uint32_t bswap32(uint32_t n)
{
	return __builtin_bswap32(n);
}
inline uint64_t bswap64(uint64_t n)
{
	return __builtin_bswap64(n);
}
#elif defined(_MSC_VER)
inline uint16_t bswap16(uint16_t n)
{
	return _byteswap_ushort(n);
}
inline uint32_t bswap32(uint32_t n)
{
	return _byteswap_ulong(n);
}
inline uint64_t bswap64(uint64_t n)
{
	return _byteswap_uint64(n);
}
#else
inline uint16_t bswap16(uint16_t n)
{
	return (
		((n & uint16_t{0xFF00}) >> 8) |
		((n & uint16_t{0x00FF}) << 8)
	);
}
inline uint32_t bswap32(uint32_t n)
{
	return (
		((n & uint32_t{0xFF000000}) >> 24) |
		((n & uint32_t{0x00FF0000}) >> 8) |
		((n & uint32_t{0x0000FF00}) << 8) |
		((n & uint32_t{0x000000FF}) << 24)
	);
}
inline uint64_t bswap64(uint64_t n)
{
	return (
		((n & uint64_t{0xFF00000000000000}) >> 56) |
		((n & uint64_t{0x00FF000000000000}) >> 40) |
		((n & uint64_t{0x0000FF0000000000}) >> 24) |
		((n & uint64_t{0x000000FF00000000}) >> 8) |
		((n & uint64_t{0x00000000FF000000}) << 8) |
		((n & uint64_t{0x0000000000FF0000}) << 24) |
		((n & uint64_t{0x000000000000FF00}) << 40) |
		((n & uint64_t{0x00000000000000FF}) << 56)
	);
}
#endif

#if defined(ENDIAN_IS_LITTLE)
inline uint16_t hton16(uint16_t host) { return bswap16(host); }
inline uint32_t hton32(uint32_t host) { return bswap32(host); }
inline uint64_t hton64(uint64_t host) { return bswap64(host); }
#elif defined(ENDIAN_IS_BIG)
inline uint16_t hton16(uint16_t host) { return host; }
inline uint32_t hton32(uint32_t host) { return host; }
inline uint64_t hton64(uint64_t host) { return host; }
#elif defined(ENDIAN_DETECT)
inline uint16_t hton16(uint16_t host) { return is_big() ? host : bswap16(host); }
inline uint32_t hton32(uint32_t host) { return is_big() ? host : bswap32(host); }
inline uint64_t hton64(uint64_t host) { return is_big() ? host : bswap64(host); }
#else
#error "Unsupported byte order: Please define ENDIAN_IS_LITTLE or ENDIAN_IS_BIG macro"
#endif

inline uint16_t ntoh16(uint16_t net) { return hton16(net); }
inline uint32_t ntoh32(uint32_t net) { return hton32(net); }
inline uint64_t ntoh64(uint64_t net) { return hton64(net); }

} // endian

#endif // NAKATT_ENDIAN_H_

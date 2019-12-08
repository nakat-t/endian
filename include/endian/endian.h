// Copyright 2019 nakat-t <armaiti.wizard@gmail.com>
// This file is subject to the license terms in the LICENSE file 
// found in the top-level directory of this distribution.

#ifndef NAKATT_ENDIAN_H_
#define NAKATT_ENDIAN_H_

#include <cstddef>
#include <cstdint>
#include <utility>

#define ENDIAN_VERSION_MAJOR 1
#define ENDIAN_VERSION_MINOR 0
#define ENDIAN_VERSION_PATCH 0
#define ENDIAN_STR(v) #v
#define ENDIAN_VERSION_(major, minor, patch) ENDIAN_STR(major) "." ENDIAN_STR(minor) "." ENDIAN_STR(patch)
#define ENDIAN_VERSION ENDIAN_VERSION_(ENDIAN_VERSION_MAJOR, ENDIAN_VERSION_MINOR, ENDIAN_VERSION_PATCH)

// auto endian detect
// @see https://en.cppreference.com/w/cpp/types/endian
#if !defined(ENDIAN_IS_LITTLE) && !defined(ENDIAN_IS_BIG)
	#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			#define ENDIAN_IS_LITTLE
		#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			#define ENDIAN_IS_BIG
		#else
			#error "endian.h support little or big endian only."
		#endif
	#elif defined(_WIN32)
		#define ENDIAN_IS_LITTLE
	#endif
#endif

namespace endian {

inline const char* version() { return ENDIAN_VERSION; }

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

template<typename T>
inline T bswap(T n)
{
	union {
		T i;
		uint8_t c[sizeof(T)];
	} x = {n};
	size_t last = sizeof(T) / 2;
	for(size_t i = 0; i < last; ++i) {
		std::swap(x.c[i], x.c[sizeof(T)-1-i]);
	}
	return x.i;
}
template<> inline uint8_t bswap<uint8_t>(uint8_t n) { return n; }
template<> inline uint16_t bswap<uint16_t>(uint16_t n) { return bswap16(n); }
template<> inline uint32_t bswap<uint32_t>(uint32_t n) { return bswap32(n); }
template<> inline uint64_t bswap<uint64_t>(uint64_t n) { return bswap64(n); }

#if defined(ENDIAN_IS_LITTLE)
inline uint16_t hton16(uint16_t host) { return bswap16(host); }
inline uint32_t hton32(uint32_t host) { return bswap32(host); }
inline uint64_t hton64(uint64_t host) { return bswap64(host); }
template<typename T> inline T hton(T host) { return bswap<T>(host); }
#elif defined(ENDIAN_IS_BIG)
inline uint16_t hton16(uint16_t host) { return host; }
inline uint32_t hton32(uint32_t host) { return host; }
inline uint64_t hton64(uint64_t host) { return host; }
template<typename T> inline T hton(T host) { return host; }
#else
inline uint16_t hton16(uint16_t host) { return is_big() ? host : bswap16(host); }
inline uint32_t hton32(uint32_t host) { return is_big() ? host : bswap32(host); }
inline uint64_t hton64(uint64_t host) { return is_big() ? host : bswap64(host); }
template<typename T> inline T hton(T host) { return is_big() ? host : bswap<T>(host); }
#endif

inline uint16_t ntoh16(uint16_t net) { return hton16(net); }
inline uint32_t ntoh32(uint32_t net) { return hton32(net); }
inline uint64_t ntoh64(uint64_t net) { return hton64(net); }
template<typename T> inline T ntoh(T net) { return hton<T>(net); }

namespace big {

#if defined(ENDIAN_IS_LITTLE)
inline uint16_t to_host16(uint16_t be) { return bswap16(be); }
inline uint32_t to_host32(uint32_t be) { return bswap32(be); }
inline uint64_t to_host64(uint64_t be) { return bswap64(be); }
template<typename T> inline T to_host(T be) { return bswap<T>(be); }
inline uint16_t from_host16(uint16_t host) { return bswap16(host); }
inline uint32_t from_host32(uint32_t host) { return bswap32(host); }
inline uint64_t from_host64(uint64_t host) { return bswap64(host); }
template<typename T> inline T from_host(T host) { return bswap<T>(host); }
#elif defined(ENDIAN_IS_BIG)
inline uint16_t to_host16(uint16_t be) { return be; }
inline uint32_t to_host32(uint32_t be) { return be; }
inline uint64_t to_host64(uint64_t be) { return be; }
template<typename T> inline T to_host(T be) { return be; }
inline uint16_t from_host16(uint16_t host) { return host; }
inline uint32_t from_host32(uint32_t host) { return host; }
inline uint64_t from_host64(uint64_t host) { return host; }
template<typename T> inline T from_host(T host) { return host; }
#else
inline uint16_t to_host16(uint16_t be) { return ntoh16(be); }
inline uint32_t to_host32(uint32_t be) { return ntoh32(be); }
inline uint64_t to_host64(uint64_t be) { return ntoh64(be); }
template<typename T> inline T to_host(T be) { return ntoh<T>(be); }
inline uint16_t from_host16(uint16_t host) { return hton16(host); }
inline uint32_t from_host32(uint32_t host) { return hton32(host); }
inline uint64_t from_host64(uint64_t host) { return hton64(host); }
template<typename T> inline T from_host(T host) { return hton<T>(host); }
#endif

} // namespace big

namespace little {

#if defined(ENDIAN_IS_LITTLE)
inline uint16_t to_host16(uint16_t le) { return le; }
inline uint32_t to_host32(uint32_t le) { return le; }
inline uint64_t to_host64(uint64_t le) { return le; }
template<typename T> inline T to_host(T le) { return le; }
inline uint16_t from_host16(uint16_t host) { return host; }
inline uint32_t from_host32(uint32_t host) { return host; }
inline uint64_t from_host64(uint64_t host) { return host; }
template<typename T> inline T from_host(T host) { return host; }
#elif defined(ENDIAN_IS_BIG)
inline uint16_t to_host16(uint16_t le) { return bswap16(le); }
inline uint32_t to_host32(uint32_t le) { return bswap32(le); }
inline uint64_t to_host64(uint64_t le) { return bswap64(le); }
template<typename T> inline T to_host(T le) { return bswap<T>(le); }
inline uint16_t from_host16(uint16_t host) { return bswap16(host); }
inline uint32_t from_host32(uint32_t host) { return bswap32(host); }
inline uint64_t from_host64(uint64_t host) { return bswap64(host); }
template<typename T> inline T from_host(T host) { return bswap<T>(host); }
#else
inline uint16_t to_host16(uint16_t le) { return is_big() ? bswap16(le) : le; }
inline uint32_t to_host32(uint32_t le) { return is_big() ? bswap32(le) : le; }
inline uint64_t to_host64(uint64_t le) { return is_big() ? bswap64(le) : le; }
template<typename T> inline T to_host(T le) { return is_big() ? bswap<T>(le) : le; }
inline uint16_t from_host16(uint16_t host) { return is_big() ? bswap16(host) : host; }
inline uint32_t from_host32(uint32_t host) { return is_big() ? bswap32(host) : host; }
inline uint64_t from_host64(uint64_t host) { return is_big() ? bswap64(host) : host; }
template<typename T> inline T from_host(T host) { return is_big() ? bswap<T>(host) : host; }
#endif

} // namespace little

} // namespace endian

#endif // NAKATT_ENDIAN_H_

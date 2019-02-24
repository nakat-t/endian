# endian

C++ header only endian conversion utility with OS independence.

# Installation

1. This library is header only. Please copy the header file to your project.
2. Define `ENDIAN_IS_LITTLE` or `ENDIAN_IS_BIG` macro before include "endian.h".
   Alternatively, if you don't define macro, it detects endianness at run time.
   In that case, a run-time endian check is added.

# Usage

Conversion between host byte order and network byte order is as follows.

```cpp
uint32_t net = endian::hton32(uint32_t{0x01020304});
uint32_t host = endian::ntoh32(net);

// template function
uint16_t net = endian::hton<uint16_t>(uint16_t{0x0102});
uint16_t host = endian::ntoh<uint16_t>(net);
```

It can convert between big endian and host, or between little endian and host.

```cpp
uint16_t be = endian::big::from_host16(uint16_t{0x0102});
uint16_t host = endian::big::to_host16(be);

uint64_t le = endian::little::from_host<uint64_t>(uint64_t{0x0102030405060708});
uint64_t host = endian::little::to_host<uint64_t>(le);
```

And, endian detection and byte swapping are:

```cpp
if(endian::is_little()) {
	u32_swap = endian::bswap32(u32_value);
}
```

For more details please see the `test/` directory.

# License

See LICENSE file.

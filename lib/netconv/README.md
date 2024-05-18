# Library for conversion of numbers from host byte order to network byte order (and back)

## What is it for?

This library provides functions for converting number from native host byte order representation to network byte order (i.e. big-endian byte order) and back. On POSIX systems (Linux, the BSDs, ...), these functions are provided by `<arpa/inet.h>` or `<netinet/in.h>`. However, these header files are not available on Ardunino. This library makes those functions (and more) available on the Arduino platform.

The following functions are provided for conversion from host byte order to network byte order:

 - `htonll()` for long long unsigned integers (uint64_t)
 - `htonl()` for long unsigned integers (uint32_t)
 - `htons()` for short unsigned integers (uint16_t)
 - `htonf()` for IEEE 754 single precision floating point numbers (32 bit)
 - `htond()` for IEEE 754 double precision floating point numbers (64 bit)

The following functions are provided for conversion from network byte order to host byte order:

 - `ntohll()` for long long unsigned integers (uint64_t)
 - `ntohl()` for long unsigned integers (uint32_t)
 - `ntohs()` for short unsigned integers (uint16_t)
 - `ntohf()` for IEEE 754 single precision floating point numbers (32 bit)
 - `ntohd()` for IEEE 754 double precision floating point numbers (64 bit)

The functions `htond()` and `ntohd()` are only available on platforms which have a "true" IEEE 754 double precision floating point implementation with 64 bits. In particular, those functions are not available on most AVR Arduinos, where the `double` data type is only an alias for the single precision 32-bit `float` data type.

## Which platform does it run on?

The library was written (and and is used by me) mainly for the Arduino platform. The code is pure C++. There is nothing in the code that prevents the library from being used on any other platform. In particular, no Arduino-specific header files are used. However, the implementation uses some special built-in functions and macros specific to gcc/g++. The library will most probably not compile with any other compiler.

## Usage

Each function is overloaded three times, i.e. there are three different versions of each function.

 - Take the number to convert as a single parameter and return the converted number, e.g `uint16_t htons(uint16_t hostshort)`.
 - Extract the number to convert from an array of bytes (uint8_t data type) and return the converted number, e.g. `uint16_t htons(uint8_t hostbytes[])`.
 - Take the number to convert as a single parameter and write the result of the conversion back to an array of bytes (uint8_t), e.g. `void htons(uint16_t hostshort, uint8_t netbytes[])`.

For details, have a look at the source code. There are extensive comments. Or generate the documentation with Doxygen.
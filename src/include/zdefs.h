#ifndef ZDEFS_H
#define ZDEFS_H

#include <stdint.h>

typedef uint8_t     byte;
typedef uint16_t    word;
typedef uint32_t    dword;
typedef uint64_t    qword;

typedef unsigned int uint;


#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
# define LITTLEENDIAN
# define endian16z(x) (x)
# define rr8(x) (x ^ 1)
#else
# define endian16z(val) ( (((val) >> 8) & 0x00ff) | (((val) << 8) & 0xff00) )
# define rr8(x) (x)
#endif

#endif // ZDEFS_H

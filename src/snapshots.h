#ifndef _SNAPSHOTS_H_
#define _SNAPSHOTS_H_
#include "z80.h"
struct TSNA{
	byte I;
	word HL1;
	word DE1;
	word BC1;
	word AF1;
	word HL,DE,BC,IY,IX;
	byte IFF;
	byte R;
	word AF,SP;
	byte IM;
	byte Border;
} __attribute__((__packed__));

#endif //_Z80_H_
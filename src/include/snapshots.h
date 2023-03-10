#ifndef _SNAPSHOTS_H_
#define _SNAPSHOTS_H_

#include "zdefs.h"

struct TSNA {
	byte I;
	word HL1, DE1, BC1, AF1;
	word HL, DE, BC, IY, IX;
	byte IFF, R;
	word AF, SP;
	byte IM, Border;
} __attribute__((__packed__));

extern "C" {
    void initSNA48k();
    void dumpSNA48k();
}

extern TSNA SNA;

#endif //_SNAPSHOTS_H_

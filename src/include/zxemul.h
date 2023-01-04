#ifndef _ZXEMUL_H_
#define _ZXEMUL_H_
#include "z80.h"

class Tzx48 {
    uint32_t * pvbuf;
    void setPixel( int line, int x, int color);
public:
	void emul();
    void init(uint32_t * pvbuf);
	int flashcounter, flash;
    void ShowZXscreen();
};


extern Tzx48 zx48;
extern void ShowZXscreen();
extern Tz80io z80io;

#endif //_ZXEMUL_H_

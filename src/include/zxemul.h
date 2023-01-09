#ifndef _ZXEMUL_H_
#define _ZXEMUL_H_
#include "z80.h"

class Tzx48 {
    dword * pvbuf;
    void setPixel( int line, int x, int color);
public:
	void emul();
    void init(dword * pvbuf);
	int flashcounter, flash;
    void ShowZXscreen();
    void ShowZXscreenNormal();
};

extern Tzx48 zx48;
extern Tz80 z80;
extern Tz80io z80io;

#endif //_ZXEMUL_H_

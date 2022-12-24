#ifndef _ZXEMUL_H_
#define _ZXEMUL_H_
#include "z80.h"

class Tzx48{
public:
	void emul();
    void init();
	int flashcounter, flash;
};

#endif //_ZXEMUL_H_

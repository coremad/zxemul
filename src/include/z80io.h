#ifndef _Z80IO_H_
#define _Z80IO_H_

#include "zdefs.h"
#include "zxkeyboard.h"


#define blines48k 240
//#define blines48k 320

class TZXports;
class TZXport;

class Tz80io {
public:
    byte zxmem[65536];
    TZXport * ports[256];

    int blines[320];

    struct TZXKeyboard ZXKeyboard;

    int bindex;
    int border;
    int kempston;

	qword tickCounter;//, opCounter;
	dword iTicks, iTicksCounter;

    Tz80io();
    void init();
    void reset();

    byte readByte(word addr);
	void writeByte(word addr, byte val);

    byte readPort(word addr);
	void writePort(word addr, byte val);

    void assignPort(word paddr, TZXport * p) {
        ports[paddr & 0xff] = p;
    };

    inline void setKey(byte krow, byte keys, bool state) {
        if (krow >= sizeof(ZXKeyboard.krows)/sizeof(byte)) return;
        if (state)
            ZXKeyboard.krows[krow] &= (~keys);
        else
            ZXKeyboard.krows[krow] |= keys;
    }

    inline void setKempston(byte but, bool state) {
        if (state)
            kempston |= but;
        else
            kempston &= (~but);
    }

    byte * pzxmem() {
        return zxmem;
    }

    int * pkempston() {
        return &kempston;
    }

//    TZXKeyboard * pZXKeyboard() {
//        return &ZXKeyboard;
//    }
};

#endif //_Z80IO_H_


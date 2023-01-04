#include "zxemul.h"

extern "C" {

uint32_t vbuf[640*480*4];

    void init() {
        zx48.init(vbuf);
    }

    byte * pzxmem() {
        return z80io.pzxmem();
    }

    int * pkempston() {
        return z80io.pkempston();
    }

    TZXKeyboard * pZXKeyboard() {
        return z80io.pZXKeyboard();
    }

    int emulStarted = 0;

    void emul() {
        if (!emulStarted) {
            emulStarted = 1;
            zx48.emul();
            zx48.ShowZXscreen();
            emulStarted = 0;
        }
    }

//    dword opCounter() {
//        return z80io.opCounter;
//    }
//
    int haltstate() {
        return z80.haltstate;
    }
    int iff1state() {
        return z80.iff1;
    }
    int iff2state() {
        return z80.iff2;
    }
    int imstate() {
        return z80.IM;
    }
    int irstate() {
        return z80.rIR;
    }

    byte opcode() {
        return z80.opcode;
    }

//    void startTape() {
//        initTape(z80io.tickCounter);
//    }
}


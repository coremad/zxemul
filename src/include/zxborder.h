#ifndef ZXBORDER_H
#define ZXBORDER_H
#include "zxports.h"

class TZXBorder : public TZXport {
public:
    TZXBorder(TZXmultiport * p) : TZXport ( p) {}

    virtual void outData(word port, byte n) {
        if ((port & 0xff) == 0xfe ) {
            int newBindex = (blines48k*z80io->iTicksCounter)/z80io->iTicks;
            if (newBindex >= z80io->bindex) {
                for (; z80io->bindex++ < newBindex; ) z80io->blines[z80io->bindex] = z80io->border;
            }
            z80io->border = n & 7; z80io->blines[z80io->bindex] = z80io->border;
        }
    }
    virtual void reset() {
        z80io->border = portdata = 7;
    }
};


#endif // ZXBORDER_H

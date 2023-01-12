#ifndef ZXBEEPER_H
#define ZXBEEPER_H

#include "zxports.h"

#define beeper_bit_mask 0xa;

#define bSetSampleRate 22050
#define abufSize (bSetSampleRate/50)

struct TZXabuffer {
    byte * aBuffer[2];
    uint playBuffer, bufpos;
};

class TZXBeeper : public TZXport {
public:
    TZXabuffer * bbuffer;
    TZXabuffer * tbuffer;

    TZXBeeper(TZXmultiport * p, TZXabuffer * b, TZXabuffer * t) : TZXport ( p) {
        bbuffer = b;
        tbuffer = t;
        t->bufpos = b->playBuffer = t->playBuffer = 0;
        for(int i = 0; i < abufSize;i++) {
            bbuffer->aBuffer[0][i] = 0x77;
            bbuffer->aBuffer[1][i] = 0x77;
            tbuffer->aBuffer[0][i] = 0x77;
            tbuffer->aBuffer[1][i] = 0x77;
        }
    }

    virtual void outData(word, byte n) {
        uint newpos = ((z80io->iTicksCounter*abufSize)/z80io->iTicks);
        if (newpos < bbuffer->bufpos) bbuffer->bufpos =0;
        for (uint i=bbuffer->bufpos; i < newpos; i++)
            bbuffer->aBuffer[bbuffer->playBuffer ^ 1][i] = portdata;
        portdata = (n >> 4)*0xff;
        bbuffer->aBuffer[bbuffer->playBuffer ^ 1][newpos] = portdata;
    }

    virtual void reset() {

    }
};

#endif // ZXBEEPER_H

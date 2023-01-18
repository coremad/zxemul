#ifndef ZXBEEPER_H
#define ZXBEEPER_H

#include "zxports.h"
//#include <stdio.h>

#define beeper_bit 4
#define beeper_bit_mask 0xa;

#define hmm 64
#define bSetSampleRate 48000
#define abufSize (bSetSampleRate/50 + hmm)

struct TZXabuffer {
    byte * aBuffer[2];
    int playBuffer, bufpos;
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
//            bbuffer->aBuffer[0][i] = 0x77;
//            bbuffer->aBuffer[1][i] = 0x77;
//            tbuffer->aBuffer[0][i] = 0x77;
//            tbuffer->aBuffer[1][i] = 0x77;
            bbuffer->aBuffer[0][i] = 128;
            bbuffer->aBuffer[1][i] = 128;
        }
    }

    virtual void outData(word, byte n) {
        int newpos = ((z80io->iTicksCounter*(abufSize - hmm))/z80io->iTicks);
        if (newpos < bbuffer->bufpos) {
            for (int i = bbuffer->bufpos; i < abufSize; i++)
                bbuffer->aBuffer[bbuffer->playBuffer][i] = portdata;
            bbuffer->bufpos = 0;
        }
        for (int i = bbuffer->bufpos; i < newpos; i++)
            bbuffer->aBuffer[bbuffer->playBuffer ^ 1][i] = portdata;
        portdata = 128+((n >> beeper_bit)&1)*63;
        bbuffer->aBuffer[bbuffer->playBuffer ^ 1][newpos] = portdata;
        for (int i = newpos; i < abufSize; i++)
            bbuffer->aBuffer[bbuffer->playBuffer ^ 1][i] = portdata;
        bbuffer->bufpos = newpos;
//        fprintf(stderr, "%02x ", portdata);
    }

    virtual void reset() {

    }
};

#endif // ZXBEEPER_H

#ifndef ZXKEMPSTON_H
#define ZXKEMPSTON_H
#ifdef DEBUG
#include <stdio.h>
#endif

#include "zxports.h"

enum fkempston { kLeft = 1, kRigth = 2, kUp = 4, kDown = 8, kFire = 16 };

class TZXKempston : public TZXport {
public:
    TZXKempston(Tz80io * p) :TZXport ( p) {
        p->assignPort(0x1f, this);
    }

    virtual byte inData(word ) {
            portdata = z80io->kempston & 0xff;
            return portdata;
        }
    virtual void reset() {
            portdata = 0;
        }

};

#endif // ZXKEMPSTON_H

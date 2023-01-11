#ifndef ZXPORTS_H
#define ZXPORTS_H

#include "z80io.h"

#ifdef DEBUG
#include <stdio.h>
#endif

class TZXmultiport;

class TZXport {

public:
    Tz80io * z80io;
    byte portdata;

    TZXport(TZXmultiport * p);
    TZXport(Tz80io * p);

    virtual byte inData(word ) { return 0; }
    virtual void outData(word , byte ) {}
    virtual void reset() {}
};


class TZXmultiport : public TZXport {
public:
    TZXport * hports[256];
    uint dcount;
    TZXmultiport(Tz80io * p, word portaddr);
    void assign(TZXport * p);
    virtual void reset();
    virtual byte inData(word p);
    virtual void outData(word p, byte d);
};


#endif // ZXPORTS_H

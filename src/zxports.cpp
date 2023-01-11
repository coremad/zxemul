#include "zxports.h"

TZXport defport((Tz80io*)nullptr);

TZXport::TZXport(Tz80io * p) {
    if(p != nullptr) z80io = p;
}

TZXport::TZXport(TZXmultiport * p) {
    if(p == nullptr) return;
    p->assign(this);
    z80io = p->z80io;
}

TZXmultiport::TZXmultiport(Tz80io * p, word portaddr) :TZXport (p) {
    dcount = 0;
    p->assignPort(portaddr & 0xff, this);
}

void TZXmultiport::assign(TZXport * p) {
    hports[dcount++] = p;
}

void TZXmultiport::reset() {
    portdata = 0;
    for (uint i = 0; i < dcount; i++) hports[i]->reset();
}

void TZXmultiport::outData(word p, byte d) {
    for (uint i = 0; i < dcount; i++) hports[i]->outData(p, d);
}

byte TZXmultiport::inData(word p) {
    portdata = 0;
    for (uint i = 0; i < dcount; i++) portdata |= hports[i]->inData(p);
    return portdata & 0xff;
}

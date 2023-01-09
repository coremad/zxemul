#ifdef DEBUG
#include <stdio.h>
#include "debug.h"
#endif
#include "snapshots.h"
#include "z80.h"
TSNA SNA;

extern Tz80 z80;

void initSNA48k() {
    z80.z80io->reset();
    z80.reset();
    z80.iff2 = z80.iff1 = SNA.IFF >> 2;
    z80.rI          = SNA.I;
    z80.rR          = SNA.R;
    z80.IM          = SNA.IM;
    z80.r16_1[rHL]  = endian16z(SNA.HL1);
    z80.r16_1[rDE]  = endian16z(SNA.DE1);
    z80.r16_1[rBC]  = endian16z(SNA.BC1);
    z80.r16[rHL]    = endian16z(SNA.HL);
    z80.r16[rDE]    = endian16z(SNA.DE);
    z80.r16[rBC]    = endian16z(SNA.BC);
    z80.r16[rIX]    = endian16z(SNA.IX);
    z80.r16[rIY]    = endian16z(SNA.IY);
    z80.r16[rSP]    = endian16z(SNA.SP);
    z80.rPC         = z80.z80io->zxmem[z80.r16[rSP]] | ((z80.z80io->zxmem[z80.r16[rSP] + 1]) << 8);
    z80.rAF1        = endian16z(SNA.AF1);
    z80.rAF         = endian16z(SNA.AF);
    z80.r16[rSP]    += 2;
    z80.z80io->border    = SNA.Border;
}

void dumpSNA48k() {
    SNA.Border      = z80.z80io->border;
    SNA.IFF         = z80.iff1 << 2;
    SNA.I           = z80.rI;
    SNA.R           = z80.rR;
    SNA.IM          = z80.IM;
    SNA.HL1         = endian16z(z80.r16_1[rHL]);
    SNA.DE1         = endian16z(z80.r16_1[rDE]);
    SNA.BC1         = endian16z(z80.r16_1[rBC]);
    SNA.AF1         = endian16z(z80.rAF1);
    SNA.HL          = endian16z(z80.r16[rHL]);
    SNA.DE          = endian16z(z80.r16[rDE]);
    SNA.BC          = endian16z(z80.r16[rBC]);
    SNA.IX          = endian16z(z80.r16[rIX]);
    SNA.IY          = endian16z(z80.r16[rIY]);
    SNA.AF          = endian16z(z80.rAF);
    SNA.SP          = endian16z(z80.r16[rSP] - 2);
    z80.z80io->zxmem[SNA.SP] = z80.rPC & 0xff;
    z80.z80io->zxmem[SNA.SP + 1] = z80.rPC >> 8;
}

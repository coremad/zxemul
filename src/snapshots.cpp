#include "snapshots.h"

TSNA SNA;

extern byte zxmem[];
extern Tz80 z80;
extern Tz80io z80io;
extern int border;

void initSNA48k() {
//    zx48.init();
    z80io.reset();
	z80.reset();
    z80.rI = SNA.I;
    z80.r16_1[rHL] = SNA.HL1;
    z80.r16_1[rDE] = SNA.DE1;
    z80.r16_1[rBC] = SNA.BC1;
    z80.rAF1 = SNA.AF1;
    z80.iff2 = z80.iff1 = SNA.IFF >> 2;
    z80.rR = SNA.R;
    z80.r16[rHL] = SNA.HL;
    z80.r16[rDE] = SNA.DE;
    z80.r16[rBC] = SNA.BC;
    z80.r16[rIX] = SNA.IX;
    z80.r16[rIY] = SNA.IY;
    z80.rAF = SNA.AF;
    z80.r16[rSP] = SNA.SP;
    z80.IM = SNA.IM;
//    z80.rPC = z80io.readByte(z80.r16[rSP]) | (z80io.readByte(z80.r16[rSP] + 1) << 8);
    z80.rPC = zxmem[z80.r16[rSP]] | ((zxmem[z80.r16[rSP] + 1]) << 8);
    z80.r16[rSP] += 2;
    border = SNA.Border;
}

void dumpSNA48k() {
    SNA.I = z80.rI;
    SNA.HL1 = z80.r16_1[rHL];
    SNA.DE1 = z80.r16_1[rDE];
    SNA.BC1 = z80.r16_1[rBC];
    SNA.AF1 = z80.rAF1;
    SNA.IFF = z80.iff1 << 2;
    SNA.R = z80.rR;
    SNA.HL = z80.r16[rHL];
    SNA.DE = z80.r16[rDE];
    SNA.BC = z80.r16[rBC];
    SNA.IX = z80.r16[rIX];
    SNA.IY = z80.r16[rIY];
    SNA.AF = z80.rAF;
    SNA.IM = z80.IM;
    SNA.SP = z80.r16[rSP] - 2;
    zxmem[SNA.SP] = z80.rPC & 0xff;
    zxmem[SNA.SP + 1] = z80.rPC >> 8;
    SNA.Border = border;
}

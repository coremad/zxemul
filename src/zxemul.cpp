#include <stdint.h>

#include "zxemul.h"
#include "snapshots.h"

extern byte zxmem[65536];
uint32_t vbuf[640*480];

Tz80io z80io;
Tz80 z80;
TSNA SNA;
Tzx48 zx48;

uint32_t itacts = 69888;

extern int bindex;
extern int border;
extern int blines[];

void Tzx48::init() {
    z80io.reset();
	z80.reset();
	flashcounter = 15;
}

void Tzx48::emul() {
    bindex = 0;
	z80.doInterrupt();
	z80.emul(itacts, itacts);
//	z80.emul(71680, 71680);
//	z80.emul(69888, 69888);
//	z80.emul(65150, 65150);
//	z80.emul(100, 100);
	if (--flashcounter == 0) {
		flash ^= 1; flashcounter = 15;
	}
    for (; bindex < 240; bindex++) blines[bindex] = border;
}

//int	zxcolors[] = {0x000000,0x00007f,0x7f0000,0x7f007f,0x007f00,0x007f7f,0x7f7f00,0x7f7f7f};
uint32_t	zxcolors[] = {0x00000000,0xffcf0000,0xff0000cf,0xffcf00cf,0xff00cf00,0xffcfcf00,0xff00cfcf,0xffcfcfcf};

inline int getAttrFColor(byte attr) {
//	return 	((zxcolors[(attr & 7)])<<((attr >> 6)&1));
    int color = zxcolors[attr & 7];
    if (color) color |= 0x303030 * ((attr >> 6) & 1);
	return 	color;
}

inline int getAttrBColor(byte attr) {
//	return 	((zxcolors[(((attr)>>3) & 7)])<<((attr >> 6)&1));
    int color = zxcolors[(attr >> 3) & 7];
    if (color) color |= 0x303030 * ((attr >> 6) & 1);
	return color;
}

int getZXpixel(int x, int y) {
	byte attr;
	word memaddr;
	attr = zxmem[22528 + y/8*32 + x/8];
	int ly = y & 63;
	int hy = y & 0xc0;
	int ofs = (-1-x) & 7;
	x >>= 3;
	memaddr = (hy + (ly>>3) + (ly & 7)*8)*32 + x;
	int pix = ((zxmem[16384 + memaddr]) >> ofs) & 1;
	if ((pix ^ (zx48.flash & (attr >> 7))) == 0) pix = getAttrBColor(attr);
	else pix = getAttrFColor(attr);
	return pix;
}

inline void setPixel( int line, int x, int color) {
    vbuf[line + x*2] = color;
    vbuf[line + x*2 + 1] = color;
    color &= 0x7fffffff;
    vbuf[line + 640 + x*2] = color;
    vbuf[line + 640 + x*2 + 1] = color;
}

void ShowZXscreen() {
    int line = 0;
    int line2 = (240-1)*640*2;
    for (int y=0; y < (240 - 192)/2; y++) {
        int c1 = zxcolors[blines[y]];
        int c2 = zxcolors[blines[240-y-1]];
        for (int x = 0; x < 640; x++) {
            vbuf[line + x] = c1;
            vbuf[line + 640 + x] = c1 & 0x7fffffff;
            vbuf[line2 + x] = c2;
            vbuf[line2 + 640 + x] = c2 & 0x7fffffff;
        }
        line += 640*2;
        line2 -= 640*2;
    }
    line = (640-256*2)/2 + (480-192*2)/2*640;
    line2 = (240 - 192)*640;
	for (int y=0; y < 192; y++) {
        int c1 = zxcolors[blines[y+(240 - 192)/2]];
        for (int x = 0; x < (320-256); x++) {
            vbuf[line2 + x] = c1;
            vbuf[line2 + x + 640 ] = c1 & 0x7fffffff;
            vbuf[line2 + 640 - x - 1] = c1;
            vbuf[line2 + 640 - x - 1 + 640] = c1 & 0x7fffffff;;
        }
		for (int x = 0; x < 256; x++) {
			int color = getZXpixel(x, y);
			setPixel(line, x, color);
		}
		line += 640*2;
		line2 += 640*2;
	}
}

extern "C" {

    void init() {
        zx48.init();
    }

    void emul() {
        zx48.emul();
        ShowZXscreen();
    }

    qword opCounter() {
        return z80io.opCounter;
    }

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
    void loadSNA48k() {
        zx48.init();
        z80.rI = SNA.I;
        z80.r16_1[rHL] = SNA.HL1;
        z80.r16_1[rDE] = SNA.DE1;
        z80.r16_1[rBC] = SNA.BC1;
        z80.rAF1 = SNA.AF1;
        z80.iff2 = z80.iff1 = SNA.IFF;
        z80.rR = SNA.R;
        z80.r16[rHL] = SNA.HL;
        z80.r16[rDE] = SNA.DE;
        z80.r16[rBC] = SNA.BC;
        z80.r16[rIX] = SNA.IX;
        z80.r16[rIY] = SNA.IY;
        z80.rAF = SNA.AF;
        z80.r16[rSP] = SNA.SP;
        z80.IM = SNA.IM;
        z80.rPC = z80io.readByte(z80.r16[rSP]) | (z80io.readByte(z80.r16[rSP] + 1) << 8);
        z80.r16[rSP] += 2;
        border = SNA.Border;
        z80.emul(itacts, itacts);
    }

    void saveSNA48k() {
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
}


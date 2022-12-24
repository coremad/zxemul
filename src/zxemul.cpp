#include <stdint.h>
#include <stddef.h>

#include "zxemul.h"
#include "snapshots.h"

extern byte zxmem[65536];
uint32_t vbuf[640*480];


Tz80io z80io;
Tz80 z80;
TSNA SNA;

Tzx48 zx48;

void Tzx48::init() {
    z80io.reset();
	z80.reset();
	flashcounter = 15;
}

void Tzx48::emul() {
	z80.doInterrupt();
	z80.emul(71680, 71680);
	if (--flashcounter == 0) {
		flash ^= 1; flashcounter = 15;
	}
}

//int	zxcolors[] = {0x000000,0x00007f,0x7f0000,0x7f007f,0x007f00,0x007f7f,0x7f7f00,0x7f7f7f};
uint32_t	zxcolors[] = {0x00000000,0xffcf0000,0xff0000cf,0xffcf00cf,0xff00cf00,0xffcfcf00,0xff00cfcf,0xffcfcfcf};

int getAttrFColor(byte attr) {
//	return 	((zxcolors[(attr & 7)])<<((attr >> 6)&1));
    int color = zxcolors[attr & 7];
    if (color) color |= 0x303030 * ((attr >> 6) & 1);
	return 	color;
}

int getAttrBColor(byte attr) {
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

extern int border;

void ShowZXscreen() {
    int bcolor = zxcolors[border];
	for (int y=0; y < (240 - 192)/2; y++) {
        int line = y*640*2;
        int line2 = (240-y-1)*640*2;
		for (int x = 0; x < 320; x++) {
			setPixel(line, x, bcolor);
			setPixel(line2, x, bcolor);
        }
	}
	for (int y=0; y < 192; y++) {
        int line2 = y*640*2 + (240 - 192)*640;
        int line = y*640*2 + (640-256*2)/2 + (480-192*2)/2*640;
		for (int x = 0; x < (320-256)/2; x++) {
			setPixel(line2, x, bcolor);
			setPixel(line2, 320 - x - 1, bcolor);
		}
		for (int x = 0; x < 256; x++) {
			int color = getZXpixel(x, y);// | x;
			setPixel(line, x, color);
		}
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
        return z80.opCounter;
    }

    byte opcode() {
        return z80.opcode;
    }
    void loadSNA48k() {
        z80.rI = SNA.I;
        z80.r16_1[rHL] = SNA.HL1;
        z80.r16_1[rDE] = SNA.DE1;
        z80.r16_1[rBC] = SNA.BC1;
        z80.rAF1 = SNA.AF1;
        z80.iff1 = SNA.IFF&1;
        z80.rR = SNA.R;
        z80.r16[rHL] = SNA.HL;
        z80.r16[rDE] = SNA.DE;
        z80.r16[rBC] = SNA.BC;
        z80.r16[rIX] = SNA.IX;
        z80.r16[rIY]=SNA.IY;
        z80.rAF = SNA.AF;
        z80.r16[rSP] = SNA.SP;
        z80.IM = SNA.IM;
        z80.rPC = z80io.readByte(z80.r16[rSP]) | (z80io.readByte(z80.r16[rSP] + 1) << 8);
        z80.r16[rSP] += 2;
        z80.iff2 = 1;
        z80.haltstate = 0;
        border = SNA.Border;
    }

    void saveSNA48k() {
        SNA.I = z80.rI;
        SNA.HL1 = z80.r16_1[rHL];
        SNA.DE1 = z80.r16_1[rDE];
        SNA.BC1 = z80.r16_1[rBC];
        SNA.AF1 = z80.rAF1;
        SNA.IFF = z80.iff1&1;
        SNA.R = z80.rR;
        SNA.HL = z80.r16[rHL];
        SNA.DE = z80.r16[rDE];
        SNA.BC = z80.r16[rBC];
        SNA.IX = z80.r16[rIX];
        SNA.IY = z80.r16[rIY];
        SNA.AF = z80.rAF;
        SNA.IM = z80.IM;
        SNA.SP = z80.r16[rSP] - 2;
        zxmem[SNA.SP + 1] = z80.rPC >> 8;
        zxmem[SNA.SP] = z80.rPC & 0xff;
        SNA.Border = border;
    }

}


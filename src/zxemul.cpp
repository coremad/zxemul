#include <stdint.h>

#include "zxemul.h"
#include "snapshots.h"
//#include "tape.h"


uint32_t vbuf[640*480];

Tz80io z80io;
Tz80 z80;
Tzx48 zx48;

uint32_t itacts = 69888;

void Tzx48::init() {
    z80io.reset();
    z80.init(&z80io);
	z80.reset();
	flashcounter = 15;
}

void Tzx48::emul() {
    z80io.bindex = 0;
	z80.doInterrupt();
	z80.emul(itacts);
//	z80.emul(71680);
//	z80.emul(69888);
//	z80.emul(65150);
	if (--flashcounter == 0) {
		flash ^= 1; flashcounter = 15;
	}
    for (; z80io.bindex < 240; z80io.bindex++) z80io.blines[z80io.bindex] = z80io.border;
}

//int	zxcolors[] = {0x000000,0x00007f,0x7f0000,0x7f007f,0x007f00,0x007f7f,0x7f7f00,0x7f7f7f};
dword	zxcolors[] = {0x00000000,0xffcf0000,0xff0000cf,0xffcf00cf,0xff00cf00,0xffcfcf00,0xff00cfcf,0xffcfcfcf};

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

inline int getZXpixel(int x, int y) {
	byte attr;
	word memaddr;
	attr = z80io.zxmem[22528 + y/8*32 + x/8];
	int ly = y & 63;
	int hy = y & 0xc0;
	int ofs = (-1-x) & 7;
	x >>= 3;
	memaddr = (hy + (ly>>3) + (ly & 7)*8)*32 + x;
	int pix = ((z80io.zxmem[16384 + memaddr]) >> ofs) & 1;
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
        int c1 = zxcolors[z80io.blines[y]];
        int c2 = zxcolors[z80io.blines[240-y-1]];
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
        int c1 = zxcolors[z80io.blines[y+(240 - 192)/2]];
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

    byte * pzxmem() {
        return z80io.pzxmem();
    }
    int * pkempston() {
        return z80io.pkempston();
    }
    TZXKeyboard * pZXKeyboard() {
        return z80io.pZXKeyboard();
    }

    int emulStarted = 0;

    void emul() {
        if (!emulStarted) {
            emulStarted = 1;
            zx48.emul();
            ShowZXscreen();
            emulStarted = 0;
        }
    }

//    dword opCounter() {
//        return z80io.opCounter;
//    }
//
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

//    void startTape() {
//        initTape(z80io.tickCounter);
//    }
}


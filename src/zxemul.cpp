#include "zxemul.h"
#include "z80.h"

Tz80io z80io;
Tz80 z80;
Tzx48 zx48;

dword itacts = 69888;

void Tzx48::init(dword * vbuf) {
    pvbuf = vbuf;
    z80io.reset();
    z80.init(&z80io);
	z80.reset();
	flashcounter = 15;
}

void Tzx48::emul() {
    z80io.bindex = 0;
	z80.doInterrupt();
	z80.emul(itacts);
	if (--flashcounter == 0) {
		flash ^= 1; flashcounter = 15;
	}
    for (; z80io.bindex < blines48k; z80io.bindex++) z80io.blines[z80io.bindex] = z80io.border;
}

//dword zxcolors[] = {0x000000,0x00007f,0x7f0000,0x7f007f,0x007f00,0x007f7f,0x7f7f00,0x7f7f7f};
#ifdef RCOLORS
dword	zxcolors[] = {0x00000000,0xffcf0000,0xff0000cf,0xffcf00cf,0xff00cf00,0xffcfcf00,0xff00cfcf,0xffcfcfcf};
#else
dword	zxcolors[] = {0x00000000,0xff0000cf,0xffcf0000,0xffcf00cf,0xff00cf00,0xff00cfcf,0xffcfcf00,0xffcfcfcf};
#endif

//#define darkline 0xff7f7f7f
#define darkline 0xff7f7f7f

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

inline void Tzx48::setPixel( int line, int x, int color) {
    pvbuf[line + x*2] = color;
    pvbuf[line + x*2 + 1] = color;
    color &= darkline;
    pvbuf[line + 640 + x*2] = color;
    pvbuf[line + 640 + x*2 + 1] = color;
}

void Tzx48::ShowZXscreen() {
    int line = 0;
    int line2 = (blines48k - 1)*640*2;
    for (int y=0; y < (blines48k - 192)/2; y++) {
        int c1 = zxcolors[z80io.blines[y]];
        int c2 = zxcolors[z80io.blines[blines48k-y-1]];
        for (int x = 0; x < 640; x++) {
            pvbuf[line + x] = c1;
            pvbuf[line + 640 + x] = c1 & darkline;
            pvbuf[line2 + x] = c2;
            pvbuf[line2 + 640 + x] = c2 & darkline;
        }
        line += 640*2;
        line2 -= 640*2;
    }
    line = (640-256*2)/2 + (480-192*2)/2*640;
    line2 = (blines48k - 192)*640;
	for (int y=0; y < 192; y++) {
        int c1 = zxcolors[z80io.blines[y+(blines48k - 192)/2]];
        for (int x = 0; x < (320-256); x++) {
            pvbuf[line2 + x] = c1;
            pvbuf[line2 + x + 640 ] = c1 & darkline;
            pvbuf[line2 + 640 - x - 1] = c1;
            pvbuf[line2 + 640 - x - 1 + 640] = c1 & darkline;
        }
		for (int x = 0; x < 256; x++) {
			int color = getZXpixel(x, y);
			setPixel(line, x, color);
		}
		line += 640*2;
		line2 += 640*2;
    }
}

void Tzx48::ShowZXscreenNormal() {
    int y1 = 24; int line = 0;
    for (int y = 0; y < 24; y++) {
        int c1 = zxcolors[z80io.blines[y]];
        for (int x = 0; x < 320; x++) pvbuf[line++] = c1;
    }
    for (int y = 0; y < 192; y++) {
        int c1 = zxcolors[z80io.blines[y1++]];
        for (int x = 0; x < 32; x++) pvbuf[line++] = c1;
        for (int x = 0; x < 256; x++) pvbuf[line++] = getZXpixel(x, y);
        for (int x = 0; x < 32; x++) pvbuf[line++] = c1;
    }
    for (int y = 192 + 24; y < blines48k; y++) {
        int c2 = zxcolors[z80io.blines[y]];
        for (int x = 0; x < 320; x++) pvbuf[line++] = c2;
    }
}

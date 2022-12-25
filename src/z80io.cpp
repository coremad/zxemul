#include "z80io.h"

byte zxmem[65536];
int blines[240];
struct TZXKeyboard ZXKeyboard;
int kempston;
int border;
int bindex;

void Tz80io::reset() {
	ZXKeyboard.kfe = ZXKeyboard.kfd = ZXKeyboard.kfb
	    = ZXKeyboard.kf7 = ZXKeyboard.kef = ZXKeyboard.kdf
	    = ZXKeyboard.kbf = ZXKeyboard.k7f = 0xbf;
	ZXKeyboard.pfe = 0;
//	bindex = 0;
}

byte Tz80io::readByte(word addr) {
	return zxmem[addr];
}

void Tz80io::writeByte(word addr, byte imm) {
	if(addr >= 16384) zxmem[addr]=imm;
}

void Tz80io::writePort(word port, byte n) {
	if ((port & 0xff) == 0xfe) {
	    qword cTicks = tickCounter % iTicks;
	    int newBindex = (240*cTicks)/iTicks;
	    if (newBindex >= bindex) {
	        for (; bindex < newBindex; bindex++) blines[bindex] = border;
	    }
	    border = n & 7; blines[bindex] = border;
	}

	if ((port == 0xfffd) || (port==0xbffd)) {
		//AY-port
		return;
	}
}

byte Tz80io::readPort(word port) {
	if ((port & 0xff) == 0xfe) {
		switch	(port) {
		case 0xfefe:
			return ZXKeyboard.kfe;
			break;
		case 0xfdfe:
			return ZXKeyboard.kfd;
			break;
		case 0xfbfe:
			return ZXKeyboard.kfb;
			break;
		case 0xf7fe:
			return ZXKeyboard.kf7;
			break;
		case 0xeffe:
			return ZXKeyboard.kef;
			break;
		case 0xdffe:
			return ZXKeyboard.kdf;
			break;
		case 0xbffe:
			return ZXKeyboard.kbf;
			break;
		case 0x7ffe:
			return ZXKeyboard.k7f;
			break;
		default:
			//printf("inport:%x\n",port);
			return ZXKeyboard.pfe;
		}
	}
	if ((port&0xff) == 31)
		return kempston & 0xff;
	if ((port&0xff) == 0xff) {
	    if ((iTicks - tickCounter % iTicks) < 17986 ) return 0xff;
	    else return zxmem[22528];
	}

	if ((port == 0xfffd) || (port == 0xbffd)) {
		//AY-port
		return 0;
	}
	return 0;
}

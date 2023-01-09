#include "z80io.h"
#include "tape.h"

void Tz80io::reset() {
	ZXKeyboard.kfe = ZXKeyboard.kfd = ZXKeyboard.kfb
	    = ZXKeyboard.kf7 = ZXKeyboard.kef = ZXKeyboard.kdf
	    = ZXKeyboard.kbf = ZXKeyboard.k7f = 0xbf;
	ZXKeyboard.pfe = 0;

	kempston = 0;
	border = 7;
	bindex = 0;
}

byte Tz80io::readByte(word addr) {
	return zxmem[addr];
}

void Tz80io::writeByte(word addr, byte imm) {
	if(addr >= 16384) zxmem[addr] = imm;
}

void Tz80io::writePort(word port, byte n) {
	if ((port & 0xff) == 0xfe) {
	    int newBindex = (blines48k*iTicksCounter)/iTicks;
	    if (newBindex >= bindex) {
	        for (; bindex++ < newBindex; ) blines[bindex] = border;
	    }
	    border = n & 7; blines[bindex] = border;
	}

	if ((port == 0xfffd) || (port==0xbffd)) {
		//AY-port
		return;
	}
}

byte Tz80io::readPort(word port) {

    byte keys;// = 191;
    switch (port & 0xff) {
	    case 0xfe:
            switch	(port) {
            case 0xfefe:
                keys = ZXKeyboard.kfe;
                break;
            case 0xfdfe:
                keys = ZXKeyboard.kfd;
                break;
            case 0xfbfe:
                keys = ZXKeyboard.kfb;
                break;
            case 0xf7fe:
                keys = ZXKeyboard.kf7;
                break;
            case 0xeffe:
                keys = ZXKeyboard.kef;
                break;
            case 0xdffe:
                keys = ZXKeyboard.kdf;
                break;
            case 0xbffe:
                keys = ZXKeyboard.kbf;
                break;
            case 0x7ffe:
                keys = ZXKeyboard.k7f;
                break;
//            case 0x00fe:
//                keys = ((!(iTicksCounter & 64)) & 0xff);
//                break;
            default:
                keys = ZXKeyboard.pfe;
            }
//            keys |= getData(tickCounter);
            keys |= iTicksCounter & 64;
            return keys;
        break;
    case 31:
        return kempston & 0xff;
        break;
    case 0xfd:
        if ((port == 0xfffd) || (port == 0xbffd)) {
            //AY-port
            return 0;
        }
        break;
    default:
    case 0xff:
            if (iTicksCounter < 17986 ) return 0xff;
            else return zxmem[22528 + (iTicksCounter & 255)];
    }
	return 0;
}

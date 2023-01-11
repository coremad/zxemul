#include "z80io.h"
#include "zxports.h"

TZXport defportz((Tz80io*)nullptr);

Tz80io::Tz80io() {
    for (uint i = 0; i < 256; i++) ports[i] = &defportz;
}

void Tz80io::init() {
    for (uint i = 0; i < 256; i++) ports[i] = &defportz;
}

void Tz80io::reset() {
    for (int i = 0; i < 256; i++) ports[i]->reset();
}

byte Tz80io::readByte(word addr) {
	return zxmem[addr];
}

void Tz80io::writeByte(word addr, byte imm) {
	if(addr >= 16384) zxmem[addr] = imm;
}

void Tz80io::writePort(word port, byte n) {
    ports[port & 0xff]->outData(port, n);
}

byte Tz80io::readPort(word port) {

    switch (port & 0xff) {
    case 0xfd:
        if ((port == 0xfffd) || (port == 0xbffd)) {
            //AY-port
            return 0;
        }
        break;
    case 0xff:
            if (iTicksCounter < 17986 ) return 0xff;
            else return zxmem[22528 + (iTicksCounter & 255)];
    default: ;
        return ports[port & 0xff]->inData(port);
    }
    return 0;
}

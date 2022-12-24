#ifndef _Z80IO_H_
#define _Z80IO_H_

#include <stdint.h>

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;
typedef unsigned int uint;

struct TZXKeyboard {
	byte kfe, kfd, kfb, kf7, kef, kdf, kbf, k7f, pfe;
};

enum fkempston{kLeft = 1, kRigth = 2, kUp = 4, kDown = 8, kFire = 16};

class Tz80io {
public:
	byte readByte(word addr);
	void writeByte(word addr, byte val);
	byte readPort(word addr);
	void writePort(word addr, byte val);
	void reset();
};

#endif //_Z80IO_H_


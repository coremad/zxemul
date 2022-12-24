#include <stdint.h>
#include "z80.h"

extern  Tz80io z80io;

inline uint parity(byte x) {
	static const byte partab[16] ={ fPV, 0, 0, fPV, 0, fPV, fPV, 0, 0, fPV, fPV, 0, fPV, 0, 0, fPV };
	return ((partab[x & 0xf] + (partab[x >> 4])) & 4)^4;
}

void Tz80::reset() {
	tickCounter = opCounter = rPC = rAF = rAF1 = rIR = xdprefix = opcode = iff1 = iff2 = haltstate = IM = 0;
	for (int i = 0; i < 6; i++) r16[i] = 0; for (int i = 0; i < 4; i++) r16_1[i] = 0;
}

inline void Tz80::addTicks(int ticks) {
	tickCounter += ticks;
}

inline word Tz80::readWord(word addr) {
	return (z80io.readByte(addr) | (z80io.readByte(addr + 1) << 8));
}

inline void Tz80::writeWord(word addr,word imm) {
		z80io.writeByte(addr, imm & 255); z80io.writeByte(addr + 1, (imm >> 8)&255);
}

inline byte Tz80::readNextByte() {
	return z80io.readByte(rPC++);
}

inline word Tz80::readNextWord() {
	word temp = readWord(rPC); rPC += 2;
	return temp ;
}

inline byte Tz80::getrr(byte rr) {
	if (rr<rrM) {
		return r8[(rr ^ 1)];
	} else if (rr==rrA){
		return rA;
	} else {
		if(xdprefix==0) {
			addTicks(3);
			return z80io.readByte(r16[rHL]);
		} else {
			if(ir==0) imm=readNextByte();
			addTicks(15);
			ir=1;
			return z80io.readByte(r16[indexreg] + imm);
		}
	}
}

inline void Tz80::setrr(byte rr, byte val) {
	if (rr<rrM) {
		r8[(rr ^ 1)]=val;
		return;
	} else if (rr==rrA) {
		rA=val;
		return;
	} else {
		if(xdprefix==0) {
			z80io.writeByte(r16[rHL],val);
			return;
		} else {
			if (ir==0) imm=readNextByte();
			z80io.writeByte(r16[indexreg] + imm,val);
			ir=0;
			return;
		}
	}
}

inline byte Tz80::getrrcb(byte rr) {
	if (rr<rrM) {
		return r8[rr ^ 1];
	} else if (rr==rrA) {
		return rA;
	} else {
		if(xdprefix == 0) {
			return z80io.readByte(r16[rHL]);
		} else {
			return z80io.readByte(r16[indexreg] + imm);
		}
	}
}

inline void Tz80::setrrcb(byte rr, byte val) {
	if (rr < rrM) {
		r8[rr ^ 1]=val;
	} else if (rr == rrA) {
		rA = val;
	} else {
		if (xdprefix == 0) {
			z80io.writeByte(r16[rHL], val);
		} else {
			z80io.writeByte(r16[indexreg] + imm, val);
		}
	}
}

inline word Tz80::get16r(byte rr) {
	if (rr != rp1HL)
		return r16[rr];
	else if (xdprefix == 0)
		return r16[rHL];
	else
		addTicks(4);
	return r16[indexreg];
}

inline void Tz80::set16r(byte rr, word nn) {
	if (rr != rp1HL)
		r16[rr] = nn;
	else if (xdprefix == 0)
		r16[rHL] = nn;
	else
		r16[indexreg] = nn;
}

inline bool	Tz80::checkCC(byte cc){
	switch (cc){
	case cNZ:
		return ((rF & fZ) == 0);
		break;
	case cZ:
		return ((rF & fZ) != 0);
		break;
	case cNC:
		return ((rF & fC) == 0);
		break;
	case cC:
		return ((rF & fC) != 0);
		break;
	case cPO:
		return ((rF & fPV) == 0);
		break;
	case cPE:
		return ((rF & fPV) != 0);
		break;
	case cP:
		return ((rF & fS) == 0);
		break;
	case cM:
		return ((rF & fS) != 0);
		break;
	default: ; //printf("error CC %x\n",cc);
		return 1;
	}
}

inline void Tz80::gotoaddr(word addr) {
	rPC=addr;
}

inline void Tz80::opINC8(byte r) {	//printf("INC8 %x\n",opcode);
	byte temp = getrr(r) + 1;
	rF=(rF & ~0xfe) | (temp & 0xa8) |
			(((temp & 0xff) == 0) << 6) |
			(((temp & 0xf) == 0) << 4) |
			((temp == 0x80) << 2);
	setrr(r, temp);
}

inline void Tz80::opDEC8(byte r) {	//printf("Dec8 %x\n",opcode);
	byte temp = getrr(r) - 1;
	rF = (rF & ~0xfe) | (temp & 0x80) |
			(((temp & 0xff) == 0) << 6) |
			(((temp & 0xf) == 0xf) << 4) |
			((temp == 0x7f) << 2) | 2;
	setrr(r, temp);
}

inline void Tz80::opLD8imm(byte r) {//printf("LD8imm ");
	if (xdprefix != 0) {
		imm = readNextByte(); ir=1;
	}
    byte temp = readNextByte();
    setrr(r, temp); addTicks(3);
}

inline void Tz80::grINCDEC16(byte rr) {
    switch (rr&1) {
	case 0://INC R16
		set16r(rr >> 1, get16r(rr>>1) + 1);
	    break;
	case 1://DEC R16
		set16r(rr >> 1, get16r(rr>>1) - 1);
	break;
    }
}

inline void Tz80::grJR(byte y) {
	int8_t offs; word temp;
	switch(y) {
	case 0: // NOP
		break;
	case 1:// EX AF,AF'
		temp = rAF; rAF = rAF1; rAF1 = temp;
		break;
	case 2:// DJNZ N
		offs = readNextByte(); addTicks(4);
		r8[rB]--;
		if (r8[rB] != 0) {
			gotoaddr(rPC + offs); addTicks(5);
		};
		break;
	case 3:// JR N
		offs = readNextByte();
		gotoaddr(rPC + offs); addTicks(8);
		break;
	default:// JR CC,N
		offs = readNextByte(); addTicks(3);
		if (checkCC(y - 4)) {
			gotoaddr(rPC + offs); addTicks(5);
		}
	}
}

inline void Tz80::gr16(byte y) {
	int p = (y>>1) & 3;
	switch(y & 1) {
	case 0://LD R16,NN
		set16r(p, readNextWord());
		break;
	case 1://ADD HL, R16
		int reghl = get16r(rHL);
		int regp = get16r(p);
		int temp = reghl + regp;
		int cbits = (reghl ^ regp ^ temp) >> 8;
		rF = (rF & ~0x3b) | (cbits & 0x10) | ((cbits >> 8) & 1);
		set16r(rHL, temp);
		break;
	}
}

inline void Tz80::grImL(byte p) {
	switch(p) {
	case 0:	// LD (BC),A
		z80io.writeByte(r16[rBC], rA);
		break;
	case 1:	// LD (DE),A
		z80io.writeByte(r16[rDE], rA);
		break;
	case 2:	// LD (NN),HL
		writeWord(readNextWord(), get16r(rHL));
		break;
	case 3:	// LD (NN),A
		z80io.writeByte(readNextWord(), rA);
		break;
	}
}

inline void Tz80::grILm(byte p) {
	switch(p) {
	case 0:	// LD A,(BC)
		rA = z80io.readByte(r16[rBC]);
		break;
	case 1:	// LD A,(DE)
		rA = z80io.readByte(r16[rDE]);
		break;
	case 2:	// LD HL,(NN)
		set16r(rHL, readWord(readNextWord()));
		break;
	case 3:	// LD A,(NN)
		rA = z80io.readByte(readNextWord());
		break;
	}
}

inline void Tz80::grIL(byte y) {
	int q=y & 1;
	int p=(y >>1) & 3;
	switch(q) {
	case 0:
		grImL(p);
		break;
	case 1:
		grILm(p);
		break;
	}
}

inline void Tz80::grX0Z7(byte y) {
	word temp=rA;
	word sum;
	int acu;
	int cbits;
	switch (y) {
	case 0: // RLCA
		rAF = ((rAF >> 7) & 0x0128) | ((rAF << 1) & ~0x1ff) |
			(rAF & 0xc4) | ((rAF >> 15) & 1);
		break;
	case 1:	// RRCA
		sum = temp >> 1;
		rAF = ((temp & 1) << 15) | (sum << 8) |
			(rAF & 0xc4) | (temp & 1);
		break;
	case 2: // RLA
		rAF = ((rAF << 8) & 0x0100) | ((rAF >> 7) & 0x28) | ((rAF << 1) & ~0x01ff) |
			(rAF & 0xc4) | ((rAF >> 15) & 1);
		break;
	case 3: // RRA
		sum = temp >> 1;
		rAF = ((rAF & 1) << 15) | (sum << 8) |
			(sum & 0x28) | (rAF & 0xc4) | (temp & 1);
		break;
	case 4:	// DAA
		acu = rA;
		temp = acu & 0xf;
		cbits = fgetC();
		if (rF & fN) {
			int hd = cbits || acu > 0x99;
			if ((rF & fH) || (temp > 9)) {
				if (temp > 5) rF=rF&(~fH);
				acu -= 6;
				acu &= 0xff;
			}
			if (hd)	acu -= 0x160;
		} else {
			if ((rF & fH) || (temp > 9)) {
				if (temp > 9) rF|=fH;
				else rF&=~fH;
				acu += 6;
			}
			if (cbits || ((acu & 0x1f0) > 0x90)) acu += 0x60;
		}
		cbits |= (acu >> 8) & 1;
		acu &= 0xff;
		rAF = (acu << 8) | (acu & 0xa8) | ((acu == 0) << 6) |
			(rAF & 0x12) | parity(acu) | cbits;
		break;
	case 5:	// CPL
		temp = temp ^ 0xff;
		rA = temp & 255;
		break;
	case 6:	// SCF
		rAF = (rAF&~0x3b)|((rAF>>8)&0x28)|1;
		break;
	case 7:	// CCF
		rAF = (rAF&~0x3b)|((rAF>>8)&0x28)|((rAF&1)<<4)|(~rAF&1);
		break;
	default: ; //printf("Error grZ7! %x\n",opcode);
	}
}

inline void Tz80::grX0(byte y, byte z) {
	switch (z) {
	case 0:
		grJR(y);
		break;
	case 1:
		gr16(y);
		break;
	case 2:
		grIL(y);
		break;
	case 3:
		grINCDEC16(y);
		break;
	case 4:
		opINC8(y);
		break;
	case 5:
		opDEC8(y);
		break;
	case 6:
		opLD8imm(y);
		break;
	case 7:
		grX0Z7(y);
		break;
	default: ;//printf("Error grX0!\n");
	}
}

inline void Tz80::opLD8(int y, int z) {
	if ((y == 6) && (z == 6)) // HALT
		haltstate = 1;
	else // LD R8,R8
		setrr(y, getrr(z));
}

inline void Tz80::setflag(byte ff){
	rF |= ff;
};

inline void Tz80::resflag(byte ff){
	rF &= (ff ^ 0xff);
};

inline byte Tz80::fgetC() {
	return(rF&1);
};

inline void Tz80::opALU(byte y, byte n){ //{aluADD,aluADC,aluSUB,aluSBC,aluAND,aluXOR,aluOR,aluCP};
	int temp,cbits;
	switch(y) {
	case aluADD:	//ADD A,
		temp = rA + n;
		cbits = rA ^ temp ^ n;
		rAF = ((temp & 0xff) << 8) | (temp & 0xa8) |
			(((temp & 0xff) == 0) << 6) | (cbits & 0x10) |
			(((cbits >> 6) ^ (cbits >> 5)) & 4) |
			((cbits >> 8) & 1);
		break;
	case aluADC:	// ADC A,
		temp = rA + n + fgetC();
		cbits = rA ^ temp ^ n;
		rAF = ((temp & 0xff) << 8) | (temp & 0xa8) |
			(((temp & 0xff) == 0) << 6) | (cbits & 0x10) |
			(((cbits >> 6) ^ (cbits >> 5)) & 4) |
			((cbits >> 8) & 1);
		break;
	case aluSUB://sub A,
		temp = rA - n;
		cbits = rA ^ n ^ temp;
		rAF = ((temp & 0xff) << 8) | (temp & 0xa8) |
			(((temp & 0xff) == 0) << 6) | (cbits & 0x10) |
			(((cbits >> 6) ^ (cbits >> 5)) & 4) | 2 |
			((cbits >> 8) & 1);
		break;
	case aluSBC:// SBC A,
		temp = rA - n - fgetC();
		cbits = rA ^ temp ^ n;
		rAF = ((temp & 0xff) << 8) | (temp & 0xa8) |
			(((temp & 0xff) == 0) << 6) | (cbits & 0x10) |
			(((cbits >> 6) ^ (cbits >> 5)) & 4) | 2 |
			((cbits >> 8) & 1);
		break;
	case aluAND:	// AND A,
		temp = rA & n;
		rAF = (temp << 8) | (temp & 0xa8) |
			((temp == 0) << 6) | 0x10 | parity(temp);
		break;
	case aluXOR:	// XOR A,
		temp = rA ^ n;
		rAF = (temp << 8) | (temp & 0xa8) | ((temp == 0) << 6) | parity(temp);
		break;
	case aluOR:	// OR A
		temp = rA | n;
		rAF = (temp << 8) | (temp & 0xa8) | ((temp == 0) << 6) | parity(temp);
		break;
	case aluCP:	// CP A
		rAF = (rAF & ~0x28) | (n & 0x28);
		temp = rA - n;
		cbits = rA ^ temp ^ n;
		rAF = (rAF & ~0xff) | (temp & 0x80) |
			(((temp & 0xff) == 0) << 6) | (n & 0x28) |
			(((cbits >> 6) ^ (cbits >> 5)) & 4) | 2 |
			(cbits & 0x10) | ((cbits >> 8) & 1);
		break;
	default: ; //printf("error opALU %x A=%x\n",n,rA);
	}
}

inline word Tz80::getrp2(byte rp2) {
	if (rp2 >= rp2AF) return rAF;
	else if(xdprefix == 0) return r16[rp2];
         else return r16[indexreg];
}

inline void Tz80::setrp2(byte rp2, word nn) {
	if (rp2 >= rp2AF) rAF = nn;
	else
		if(xdprefix == 0) r16[rp2] = nn;
		else r16[indexreg] = nn;
}

inline void Tz80::opPUSH(word nn) {
	r16[rSP] -= 2;
	writeWord(r16[rSP], nn);
}

inline void Tz80::bLD() {
	int acu = z80io.readByte(r16[rHL]);
	z80io.writeByte(r16[rDE], acu);
	acu += rA;
	rF = (rF & ~0x3e) | (acu & 8) | ((acu & 2) << 4) |
		(((--r16[rBC] & 0xffff) != 0) << 2);
	addTicks(12);
}

inline void Tz80::bCP() {
	int acu = rA;
	int temp = z80io.readByte(r16[rHL]);
	int sum = acu - temp;
	int cbits = acu ^ temp ^ sum;
	rAF = (rAF & ~0xfe) | (sum & 0x80) | (!(sum & 0xff) << 6) |
		(((sum - ((cbits&16)>>4))&2) << 4) | (cbits & 16) |
		((sum - ((cbits >> 4) & 1)) & 8) |
		((--r16[rBC] & 0xffff) != 0) << 2 | 2;
	if ((sum & 15) == 8 && (cbits & 16) != 0)
	rAF &= ~8;
	addTicks(12);
}

inline void Tz80::grBLI(byte a, byte b) {
	switch(b) {
	case 0:
		bLD();
		switch(a) {
		case 4:// LDI
			r16[rHL]++; r16[rDE]++;
			break;
		case 5:// LDD
			r16[rHL]--; r16[rDE]--;
			break;
		case 6:// LDIR
			r16[rHL]++; r16[rDE]++;
			if (r16[rBC] > 0) gotoaddr(rPC-2);
			break;
		case 7:// LDDR
			r16[rHL]--; r16[rDE]--;
			if (r16[rBC] > 0) gotoaddr(rPC-2);
			break;
		default: ; //printf("grBLI error! %x a=%x b=%x\n",opcode,a,b);
		}
		break;
	case 1:
		bCP();
		switch(a) {
		case 4:// CPI
			r16[rHL]++;
			break;
		case 5:// CPD
			r16[rHL]--;
			break;
		case 6:// CPIR
			r16[rHL]++;
			if ((r16[rBC] > 0) && ((rF & fZ) == 0)) gotoaddr(rPC-2);
			break;
		case 7:// CPDR
			r16[rHL]--;
			if ((r16[rBC] > 0) && ((rF & fZ) == 0)) gotoaddr(rPC-2);
			break;
		default: ; //printf("grBLI error! %x a=%x b=%x\n",opcode,a,b);
		}
		break;
	default: ; //printf("grBLI error! %x a=%x b=%x\n",opcode,a,b);
	}
}

inline void Tz80::opIM(byte y) { //printf("PC=%x IM y=%x  %x \n",rPC, y, opcode);
	IM = y-1; addTicks(4);
}

inline void Tz80::grEDX1(byte y, byte z) {
	byte p,q;
	q = (y & 1);
	p = ((y>>1) & 7);
	dword temp;
	byte temp1;
	int cbits;
	int acu;
	switch(z) {
	case 0:
		switch(y) {
		case 6:// IN X,(C)
			temp1 = z80io.readPort(r16[rBC]);
			rF = (rF & ~0xfe) | (temp1 & 0xa8) |
				(((temp1 & 0xff) == 0) << 6) |
				parity(temp1);
			break;
		default:// IN R,(C)
			temp1 = z80io.readPort(r16[rBC]);
			setrr(y, temp1);
			rF = (rF & ~0xfe) | (temp1 & 0xa8) |
				(((temp1 & 0xff) == 0) << 6) |
				parity(r8[y]);
			break;
		}
		break;
	case 1:
		switch(y) {
		case 6:// OUT (C),0
			z80io.writePort(r16[rBC], 0);
			break;
		default:// OUT (C),R
			z80io.writePort(r16[rBC], getrr(y));
			break;
		}
		break;
	case 2:
		switch (q) {
		case 0: // SBC HL,rp
			temp = r16[rHL] - r16[p] - fgetC();
			cbits = (r16[rHL] ^ r16[p] ^ temp) >> 8;
			r16[rHL] = temp;
			rF = ((temp >> 8) & 0xa8) |
				(((temp & 0xffff) == 0) << 6) |
				(((cbits >> 6) ^ (cbits >> 5)) & 4) |
				(cbits & 0x10) | 2 | ((cbits >> 8) & 1);
			addTicks(15);
			break;
		case 1: // ADC HL,rp
			temp = r16[rHL] + r16[p] + fgetC();
			cbits = (r16[rHL] ^ r16[p] ^ temp) >> 8;
			r16[rHL] = temp;
			rF =  ((temp >> 8) & 0xa8) |
				(((temp & 0xffff) == 0) << 6) |
				(((cbits >> 6) ^ (cbits >> 5)) & 4) |
				(cbits & 0x10) | ((cbits >> 8) & 1);
			addTicks(15);
			break;
		}
		break;
	case 3:
		switch (q) {
		case 0:// LD (NN),rp
			writeWord(readNextWord(), r16[p]);
			break;
		case 1:// LD rp,(NN)
			r16[p] = readWord(readNextWord());
			break;
		};
		break;
	case 4:	// NEG
		temp = rA;
		rA = -rA;
		rAF |= (rA & 0xa8) | (((rAF & 0xff00) == 0) << 6) |
			(((temp & 0x0f) != 0) << 4) | ((temp == 0x80) << 2) |
			2 | (temp != 0);
		break;
	case 5:	//RETN //RETI
		gotoaddr(opPOP());
		iff1 = iff2;
		break;
	case 6:	opIM(y);
		break;
	case 7:
		switch(y) {
		case 0:	// LD I,A
			rI = rA;
			break;
		case 1:	// LD R,A
			rR = rA;
			break;
		case 2:	// LD A,I
			if (iff2 != 0)
				setflag(fPV);
			else
				resflag(fPV);
			rA = rI;
			break;
		case 3:	// LD A,R
			rA = rR;
			break;
		case 5:	// RLD
			temp = z80io.readByte(r16[rHL]);
			acu = rA;
			z80io.writeByte(r16[rHL], ((temp&0xf) << 4) | (acu & 0xf));
			acu = (acu & 0xf0) | ((temp>>4)&0xf);
			rAF = (acu << 8) | (acu & 0xa8) | (((acu & 0xff) == 0) << 6) |
				parity(acu) | (rAF & 1);
			break;
		default: ; //printf("grEDX1Z7 error! %x y=%x z=%x\n",opcode,y,z);
		}
		break;
	default: ; //printf("grEDX1 error! %x y=%x z=%x\n",opcode,y,z);
	};
};

inline void Tz80::grED() {
	byte x,y,z;
	opcode = readNextByte();
	z = opcode & 7;
	y = (opcode >> 3) & 7;
	x = (opcode >> 6) & 3;
	switch(x) {
		case 1:
			grEDX1(y,z);
			break;
		case 2:
			grBLI(y,z);
			break;
	default: ; //printf("ED preffix error! %x x=%x y=%x z=%x \n",opcode,x,y,z);
	}
}

inline void Tz80::grPUSH(byte y) {
	byte p,q; word temp;
	q = y & 1;
	p = (y >> 1) & 3;
	switch (q) {
	case 0:// PUSH rp
		opPUSH(getrp2(p));
		break;
	case 1:
		switch(p) {
		case 0:// CALL NN
			temp = readNextWord();
			opPUSH(rPC);
			gotoaddr(temp);
			addTicks(17);
			break;
		case 1:// DD prefix
			xdprefix = 0xdd; indexreg = rIX;
			break;
		case 2:// ED prefix
			grED();
			break;
		case 3:// FD prefix
			xdprefix = 0xfd;
			indexreg = rIY;
			break;
		}
		break;
	}
}

inline word Tz80::opPOP() {
	word temp = readWord(r16[rSP]);
	r16[rSP] += 2;
	return temp;
}

inline void Tz80::opEXX() {
	word tempbc = r16[rBC]; r16[rBC] = r16_1[rBC]; r16_1[rBC] = tempbc;
	word tempde = r16[rDE]; r16[rDE] = r16_1[rDE]; r16_1[rDE] = tempde;
	word temphl = r16[rHL]; r16[rHL] = r16_1[rHL]; r16_1[rHL] = temphl;
}

inline void Tz80::grPOP(byte y) {
	byte p, q;
	q = (y & 1);
	p = ((y >> 1) & 3);
	switch(q) {
	case 0:// POP rp
		setrp2(p,opPOP());
		break;
	case 1:
		switch (p) {
		case 0:// RET
			gotoaddr(opPOP());
			addTicks(6);
			break;
		case 1:// EXX
			opEXX();
			break;
		case 2:// JP HL
			gotoaddr(get16r(rHL));
			break;
		case 3:// LD SP,HL
			r16[rSP]=get16r(rHL);
			break;
		}
		break;
	}
}

inline void Tz80::rotateCB(byte y, byte z) {//{rotRLC,rotRRC,rotRL,rotRR,rotSLA,rotSRA,rotSLL,rotSRL};
	int temp, cbits, reg;
	reg = getrrcb(z);
	switch(y) {
	case rotRLC:
		temp = (reg << 1) | (reg >> 7);
		cbits = temp & 1;
		break;
	case rotRRC:
		temp = (reg >> 1) | (reg << 7);
		cbits = temp & 0x80;
		break;
	case rotRL:
		temp = (reg << 1) | fgetC();
		cbits = reg & 0x80;
		break;
	case rotRR:
		temp = (reg >> 1) | (fgetC() << 7);
		cbits = reg & 1;
		break;
	case rotSLA:
		temp = reg << 1;
		cbits = reg & 0x80;
		break;
	case rotSRA:
		temp = (reg >> 1) | (reg & 0x80);
		cbits = reg & 1;
		break;
	case rotSLL:
        temp = (reg << 1) | 1;
        cbits = reg & 0x80;
		break;
	case rotSRL:
		temp = reg >> 1;
		cbits = reg & 1;
		break;
	default:
		temp = 0;
		cbits = 0;
//		printf("error rotate  %x\n",opcode);
	}
	setrrcb(z, temp);
	rF = (temp & 0xa8) |
		(((temp & 0xff) == 0) << 6) |
		parity(temp) | !!cbits;
}

inline void Tz80::grCB() {
	byte x, y, z, acu;
	if (xdprefix != 0) imm = readNextByte();
	opcode = readNextByte();
	z = opcode & 7;
	y = (opcode >> 3) & 7;
	x = (opcode >> 6) & 3;
	switch (x) {
	case 0:
		rotateCB(y, z);
		break;
	case 1:// BIT y,z;
		acu = getrrcb(z);
		if (acu & (1 << y))
			rF = (rF & ~0xfe) | 0x10 |((y == 7) << 7);
		else
			rF = (rF & ~0xfe) | 0x54;
		break;
	case 2: // RES y,z
		setrrcb(z, getrrcb(z) & (~(1 << y)));
		break;
	case 3:// opSET y,z
		setrrcb(z, getrrcb(z) | (1 << y));
		break;
	default: ; //printf("error CB prefix %x\n",opcode);
	}
}

inline void Tz80::grJP(byte y) {
	word temp;
	switch(y) {
	case 0:// JP NN
		addTicks(6); gotoaddr(readNextWord());
		break;
	case 1:// CB PREFIX
		grCB();
		break;
	case 2:// OUT (N),A
		z80io.writePort(readNextByte() | (rA<<8), rA);
		break;
	case 3:// IN A,(N)
		rA = z80io.readPort(readNextByte() | (rA<<8));
		break;
	case 4:// EX (SP),HL
		temp = get16r(rHL);
		set16r(rHL, readWord(r16[rSP]));
		writeWord(r16[rSP], temp);
		break;
	case 5:// EX DE,HL
		temp = r16[rDE]; r16[rDE] = r16[rHL]; r16[rHL] = temp;
		break;
	case 6:// DI
		iff1 = 0; iff2 = 0;
		break;
	case 7:// EI
		iff1 = 1; iff2 = 1;
		break;
	default: ; //printf("Error grJP! %x\n",opcode);
	}
}

inline void Tz80::grX3(byte y, byte z) {
	word temp;
	switch (z) {
	case 0:// RET CC
		if (checkCC(y)) gotoaddr(opPOP());
		break;
	case 1:
		grPOP(y);
		break;
	case 2:// JP CC,NN
		temp=readNextWord();addTicks(6);
		if (checkCC(y)) gotoaddr(temp);
		break;
	case 3:
		 grJP(y);
		break;
	case 4:// CALL CC,NN
		temp = readNextWord(); addTicks(10);
		if (checkCC(y)) {
			opPUSH(rPC);
			gotoaddr(temp);
			addTicks(17-10);
		}
		break;
	case 5:
		grPUSH(y);
		break;
	case 6:
		opALU(y,readNextByte());
		addTicks(7);
		break;
	case 7:// RST N
		opPUSH(rPC);
		gotoaddr(y*8);
		addTicks(11);
		break;
	default: ; //printf("Error grX3! %x\n",opcode);
	}
}

int Tz80::emul(dword opNum, dword tickNum) {
	qword endOp = opNum + opCounter;
	qword endTick = tickNum + tickCounter;
	do {
		if(haltstate) break;
		opcode=readNextByte();
		int z = opcode & 7;
		int y = (opcode >> 3) & 7;
		int x = (opcode >> 6) & 3;
		switch (x) {
        case 0:
            grX0(y, z);
            break;
        case 1:
            opLD8(y, z);
            break;
        case 2:
            opALU(y, getrr(z));
            break;
        case 3:
            grX3(y, z);
            break;
        default: ; //printf("Error opcode %x\n",opcode);
		}
		rR++;
		if (xdprefix != 0) {
			if((opcode != 0xdd) && (opcode != 0xfd)) {
				xdprefix = 0; ir = 0;
			} else continue;
		}
		opCounter++; tickCounter += 4;
	} while((opCounter < endOp) && (tickCounter < endTick));
	return 0;
}

void Tz80::doInterrupt() {
	if (iff1 != 0) {
		opPUSH(rPC);
		if (IM == 2) rPC = readWord((rIR & 0xff00) + 255);
		else rPC = 0x38;
		haltstate = 0;
		iff1 = iff2 = 0;
	}
}

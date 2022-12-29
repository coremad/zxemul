#ifndef _Z80_H_
#define _Z80_H_

#include "z80io.h"

enum regs8{rC, rB, rE, rD, rL, rH, rSPL, SPH, rXL, rXH, rYL, rYH};
enum regs16{rBC, rDE, rHL, rSP, rIX, rIY};

enum z80cc {cNZ, cZ, cNC, cC, cPO, cPE, cP, cM};
enum z80flags {fC=1, fN=2, fPV=4, fF3=8, fH=16, fF5=32, fZ=64, fS=128};

enum rp1 {rp1BC, rp1DE, rp1HL, rp1SP};
enum rp2 {rp2BC, rp2DE, rp2HL, rp2AF};

enum rr {rrC, rrB, rrE, rrD, rrL, rrH, rrM, rrA};
enum alu {aluADD, aluADC, aluSUB, aluSBC, aluAND, aluXOR, aluOR, aluCP};

enum pCB {rotCB, bitCB, resCB, setCB};
enum rot {rotRLC, rotRRC, rotRL, rotRR, rotSLA, rotSRA, rotSLL, rotSRL};

class Tz80 {
public:
	word rPC;
	union {
		word rAF;
		struct {
            byte rF;
            byte rA;
		};
	};
	union {
		byte r8[12];
		word r16[6];
	};
	union {
		word rAF1;
		struct{
            byte rF1;
            byte rA1;
		};
	};
	union {
		byte r8_1[8];
		word r16_1[4];
	};
	union {
		word rIR;
		struct {
            byte rR;
            byte rI;
		};
	};
	int iff1, iff2;
	int IM;
	int haltstate;

	byte opcode;
	byte xdprefix;
	byte cbprefix;
	byte edprefix;
	word addr;
	int8_t imm;
	int indexreg;
	int ir;

	void reset();
	int emul(dword opNum, dword tickNum);
	void doInterrupt();
	void setflag(byte ff);
	void resflag(byte ff);

private:
	byte readByte(word addr);
	void writeByte(word addr, byte val);
	byte readPort(word addr);
	void writePort(word addr, byte val);
	word opPOP();
	void opPUSH(word nn);
	word readWord(word  addr);
	void writeWord(word addr, word imm);
	byte readNextByte();
	word readNextWord();

	void addTicks(int ticks);
    void incR(int val);

	byte getrr(byte rr);
	byte getrrN(byte rr);
	byte getrrO(byte rr);
	byte getrrL(byte rr);

	void setrr(byte rr, byte val);
	void setrrN(byte rr, byte val);
	void setrrO(byte rr, byte val);
    void setrrL(byte rr, byte val);

	byte getrrcb(byte rr);
	void setrrcb(byte r, byte val);

	word get16r(byte rr);
	void set16r(byte rr, word nn);

	bool checkCC(byte cc);
	void opNOP();
	void opEX_AF_AF();
	void opDJNZ();
	void gotoaddr(word addr);
    void gotooffs(int8_t offs);
	void opINC16(byte rr);
	void opDEC16(byte rr);
	void opINC8(byte r);
	void opDEC8(byte r);
	void opLD8imm(byte r);
	void grINCDEC16(byte rr);
	void grJR(byte y);
	void gr16(byte y);
	void grImL(byte p);
	void grILm(byte p);
	void grIL(byte y);
	void grX0Z7(byte y);
	void grX0(byte y, byte z);
	void opLD8(int y, int z);
	void opALU(byte y, byte n);
	byte fgetC();
	word getrp2(byte rp2);
	void setrp2(byte rp2, word nn);
	void opCALL();
	void bLD();
	void bCP();
	void grBLI(byte a, byte b);
	void opIM(byte y);
	void grEDX1(byte y, byte z);
	void fsetZ(int n);
	void grED();
	void grPUSH(byte y);
	void opEXX();
	void grPOP(byte y);
	void opSET(byte y, byte z);
	void opRES(byte y, byte z);
	void opBIT(byte y, byte z);
	void rotateCB(byte y, byte z, byte val);
	void grCB();
	void grJP(byte y);
	void grX3(byte y, byte z);
	void errorpref();
};

extern byte zxmem[];

#endif //_Z80_H_


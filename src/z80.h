#ifndef _Z80_H_
#define _Z80_H_

#include "z80io.h"

enum regs8{rC,rB,rE,rD,rL,rH,rXL,rXH,rYL,rYH};
enum regs16{rBC,rDE,rHL,rSP,rIX,rIY};

enum z80cc {cNZ,cZ,cNC,cC,cPO,cPE,cP,cM};
enum z80flags {fC=1,fN=2,fPV=4,fF3=8,fH=16,fF5=32,fZ=64,fS=128};

enum rp1 {rp1BC,rp1DE,rp1HL,rp1SP};
enum rp2 {rp2BC,rp2DE,rp2HL,rp2AF};

enum rr {rrC,rrB,rrE,rrD,rrL,rrH,rrM,rrA};
enum alu {aluADD,aluADC,aluSUB,aluSBC,aluAND,aluXOR,aluOR,aluCP};

enum rot {rotRLC,rotRRC,rotRL,rotRR,rotSLA,rotSRA,rotSLL,rotSRL};

class Tz80{
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
	qword tickCounter, opCounter;

	byte opcode;
	byte cbprefix;
	byte xdprefix;
	word addr;
	int8_t	imm;
	int indexreg;
	int ir;

	int iff1,iff2;
	int IM;
	int haltstate;


	int debug;

	word opPOP();
	void opPUSH(word nn);
	void reset();
	int emul(dword opNum, dword tickNum);
	void doInterrupt();

private:
	word readWord(word  addr);
	void writeWord(word addr, word imm);
	void addTicks(int ticks);
	byte readNextByte();
	word readNextWord();
	byte getrr(byte r);
	void setrr(byte rr, byte val);
	byte getrrcb(byte rr);
	void setrrcb(byte r, byte val);
	word get16r(byte rr);
	void set16r(byte rr, word nn);
	bool checkCC(byte cc);
	void opNOP();
	void opEX_AF_AF();
	void opDJNZ();
	void gotoaddr(word addr);
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
	void setflag(byte ff);
	void resflag(byte ff);
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
	void rotateCB(byte y, byte z);
	void grCB();
	void grJP(byte y);
	void grX3(byte y, byte z);
	void errorpref();
};

#endif //_Z80_H_


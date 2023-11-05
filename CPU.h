#pragma once
#include <stdio.h>
#include <math.h>
#include <malloc.h>

using Byte8 = unsigned char;
using Byte16 = unsigned short;

static constexpr Byte8
LDA_IM = 0xA9,
LDA_ZP = 0xA5,
LDA_ZPX = 0xB5,
LDA_AB = 0xAD,
LDA_ABX = 0xBD,
LDA_ABY = 0xB9,
// TODO
LDA_INX = 0xA1,
LDA_INY = 0xB1;

static constexpr Byte8
LDX_IM = 0xA2,
LDX_ZP = 0xA6,
LDX_ZPY = 0xB6,
LDX_AB = 0xAE,
LDX_ABY = 0xBE;

static constexpr Byte8
LDY_IM = 0xA0,
LDY_ZP = 0xA4,
LDY_ZPX = 0xB4,
LDY_AB = 0xAC,
LDY_ABX = 0xBC;

static constexpr Byte8
STA_ZP = 0x85,
STA_ZPX = 0x95,
STA_AB = 0x8D,
STA_ABX = 0x9D,
STA_ABY = 0x99,
// TODO
STA_INX = 0x81,
STA_INY = 0x91;

static constexpr Byte8
STX_ZP = 0x86,
STX_ZPY = 0x96,
STX_AB = 0x8E;

static constexpr Byte8
STY_ZP = 0x84,
STY_ZPX = 0x94,
STY_AB = 0x8C;

static constexpr Byte8 TAX = 0xAA;
static constexpr Byte8 TAY = 0xA8;
static constexpr Byte8 TXA = 0x8A;
static constexpr Byte8 TYA = 0x98;

static constexpr Byte8 TSX = 0xBA;
static constexpr Byte8 TXS = 0x9A;

static constexpr Byte8 PHA = 0x48;
static constexpr Byte8 PHP = 0x08;

static constexpr Byte8 PLA = 0x68;
static constexpr Byte8 PLP = 0x28;

class Memory
{
public :
	static constexpr unsigned int MAX_MEM = 0xFFFF;
	Byte8* data;

	Memory();
	~Memory();
};

class CPU
{
public:
	Byte16 programCounter;

	// All the registers for the CPU
	Byte8 accumulator, registerX, registerY, stackPointer;

	Byte8 carryFlag : 1;
	Byte8 zeroFlag : 1;
	Byte8 interruptDisable : 1;
	Byte8 decimalMode : 1;
	Byte8 breakCommand : 1;
	Byte8 overflowFlag : 1;
	Byte8 negativeFlag : 1;
	Byte8 bFlag : 1;

	CPU( Memory& memory);
	~CPU();
	// void reset();
	Byte8 FetchByte(Memory& memory);
	Byte16 FetchByte16(Memory& memory);
	Byte8 ReadByte(Memory& memory, Byte16 address);
	Byte8 ReadByteFromStack(Memory& memory);
	void WriteByte(Memory& memory, Byte8 address, Byte8& regist);
	void WriteByteToStack(Memory& memory, Byte8& address, Byte8& regist);

	void Execute(Memory& memory);
	void Accum_FLAGS();
	void LDX_TSX_FLAGS();
	void LDY_FLAGS();
	void TAX_FLAGS();
	void TAY_FLAGS();
	void TXA_TYA_FLAGS();

};
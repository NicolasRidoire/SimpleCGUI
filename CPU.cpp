/*
	CPU - An emulation of the 6502/2A03 processor
	"Thanks Dad for believing computers were gonna be A big deal..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Background
	~~~~~~~~~~
	I love this microprocessor. It was at the heart of two of my favourite
	machines, the BBC Micro, and the Nintendo Entertainment System, as well
	as countless others in that era. I learnt to program on the Model B, and
	I learnt to love games on the NES, so in many ways, this processor is
	why I am the way I am today.

	In February 2019, I decided to undertake A selfish personal project and
	build A NES emulator. Ive always wanted to, and as such I've avoided
	looking at source code for such things. This made making this A real
	personal challenge. I know its been done countless times, and very likely
	in far more clever and accurate ways than mine, but I'm proud of this.

	Datasheet: http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

	Files: CPU.h, CPU.cpp

	Relevant Video:

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/

#include "CPU.h"
#include "Bus.h"

// Constructor
CPU::CPU()
{
	// Assembles the translation table. It's big, it's ugly, but it yields A convenient way
	// to emulate the 6502. I'm certain there are some "code-golf" strategies to reduce this
	// but I've deliberately kept it verbose for study and alteration

	// It is 16x16 entries. This gives 256 instructions. It is arranged to that the bottom
	// 4 bits of the instruction choose the column, and the top 4 bits choose the row.

	// For convenience to get function pointers to members of this class, I'm using this
	// or else it will be much much larger :D

	// The table is one big initialiser list of initialiser lists...
	using A = CPU;
	lookup =
	{
		{ "BRK", &A::BRK, &A::IMM, 7 },{ "ORA", &A::ORA, &A::IZX, 6 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 3 },{ "ORA", &A::ORA, &A::ZP0, 3 },{ "ASL", &A::ASL, &A::ZP0, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "PHP", &A::PHP, &A::IMP, 3 },{ "ORA", &A::ORA, &A::IMM, 2 },{ "ASL", &A::ASL, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::NOP, &A::IMP, 4 },{ "ORA", &A::ORA, &A::ABS, 4 },{ "ASL", &A::ASL, &A::ABS, 6 },{ "???", &A::XXX, &A::IMP, 6 },
		{ "BPL", &A::BPL, &A::REL, 2 },{ "ORA", &A::ORA, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 4 },{ "ORA", &A::ORA, &A::ZPX, 4 },{ "ASL", &A::ASL, &A::ZPX, 6 },{ "???", &A::XXX, &A::IMP, 6 },{ "CLC", &A::CLC, &A::IMP, 2 },{ "ORA", &A::ORA, &A::ABY, 4 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 7 },{ "???", &A::NOP, &A::IMP, 4 },{ "ORA", &A::ORA, &A::ABX, 4 },{ "ASL", &A::ASL, &A::ABX, 7 },{ "???", &A::XXX, &A::IMP, 7 },
		{ "JSR", &A::JSR, &A::ABS, 6 },{ "AND", &A::AND, &A::IZX, 6 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "BIT", &A::BIT, &A::ZP0, 3 },{ "AND", &A::AND, &A::ZP0, 3 },{ "ROL", &A::ROL, &A::ZP0, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "PLP", &A::PLP, &A::IMP, 4 },{ "AND", &A::AND, &A::IMM, 2 },{ "ROL", &A::ROL, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "BIT", &A::BIT, &A::ABS, 4 },{ "AND", &A::AND, &A::ABS, 4 },{ "ROL", &A::ROL, &A::ABS, 6 },{ "???", &A::XXX, &A::IMP, 6 },
		{ "BMI", &A::BMI, &A::REL, 2 },{ "AND", &A::AND, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 4 },{ "AND", &A::AND, &A::ZPX, 4 },{ "ROL", &A::ROL, &A::ZPX, 6 },{ "???", &A::XXX, &A::IMP, 6 },{ "SEC", &A::SEC, &A::IMP, 2 },{ "AND", &A::AND, &A::ABY, 4 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 7 },{ "???", &A::NOP, &A::IMP, 4 },{ "AND", &A::AND, &A::ABX, 4 },{ "ROL", &A::ROL, &A::ABX, 7 },{ "???", &A::XXX, &A::IMP, 7 },
		{ "RTI", &A::RTI, &A::IMP, 6 },{ "EOR", &A::EOR, &A::IZX, 6 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 3 },{ "EOR", &A::EOR, &A::ZP0, 3 },{ "LSR", &A::LSR, &A::ZP0, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "PHA", &A::PHA, &A::IMP, 3 },{ "EOR", &A::EOR, &A::IMM, 2 },{ "LSR", &A::LSR, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "JMP", &A::JMP, &A::ABS, 3 },{ "EOR", &A::EOR, &A::ABS, 4 },{ "LSR", &A::LSR, &A::ABS, 6 },{ "???", &A::XXX, &A::IMP, 6 },
		{ "BVC", &A::BVC, &A::REL, 2 },{ "EOR", &A::EOR, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 4 },{ "EOR", &A::EOR, &A::ZPX, 4 },{ "LSR", &A::LSR, &A::ZPX, 6 },{ "???", &A::XXX, &A::IMP, 6 },{ "CLI", &A::CLI, &A::IMP, 2 },{ "EOR", &A::EOR, &A::ABY, 4 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 7 },{ "???", &A::NOP, &A::IMP, 4 },{ "EOR", &A::EOR, &A::ABX, 4 },{ "LSR", &A::LSR, &A::ABX, 7 },{ "???", &A::XXX, &A::IMP, 7 },
		{ "RTS", &A::RTS, &A::IMP, 6 },{ "ADC", &A::ADC, &A::IZX, 6 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 3 },{ "ADC", &A::ADC, &A::ZP0, 3 },{ "ROR", &A::ROR, &A::ZP0, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "PLA", &A::PLA, &A::IMP, 4 },{ "ADC", &A::ADC, &A::IMM, 2 },{ "ROR", &A::ROR, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "JMP", &A::JMP, &A::IND, 5 },{ "ADC", &A::ADC, &A::ABS, 4 },{ "ROR", &A::ROR, &A::ABS, 6 },{ "???", &A::XXX, &A::IMP, 6 },
		{ "BVS", &A::BVS, &A::REL, 2 },{ "ADC", &A::ADC, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 4 },{ "ADC", &A::ADC, &A::ZPX, 4 },{ "ROR", &A::ROR, &A::ZPX, 6 },{ "???", &A::XXX, &A::IMP, 6 },{ "SEI", &A::SEI, &A::IMP, 2 },{ "ADC", &A::ADC, &A::ABY, 4 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 7 },{ "???", &A::NOP, &A::IMP, 4 },{ "ADC", &A::ADC, &A::ABX, 4 },{ "ROR", &A::ROR, &A::ABX, 7 },{ "???", &A::XXX, &A::IMP, 7 },
		{ "???", &A::NOP, &A::IMP, 2 },{ "STA", &A::STA, &A::IZX, 6 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 6 },{ "STY", &A::STY, &A::ZP0, 3 },{ "STA", &A::STA, &A::ZP0, 3 },{ "STX", &A::STX, &A::ZP0, 3 },{ "???", &A::XXX, &A::IMP, 3 },{ "DEY", &A::DEY, &A::IMP, 2 },{ "???", &A::NOP, &A::IMP, 2 },{ "TXA", &A::TXA, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "STY", &A::STY, &A::ABS, 4 },{ "STA", &A::STA, &A::ABS, 4 },{ "STX", &A::STX, &A::ABS, 4 },{ "???", &A::XXX, &A::IMP, 4 },
		{ "BCC", &A::BCC, &A::REL, 2 },{ "STA", &A::STA, &A::IZY, 6 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 6 },{ "STY", &A::STY, &A::ZPX, 4 },{ "STA", &A::STA, &A::ZPX, 4 },{ "STX", &A::STX, &A::ZPY, 4 },{ "???", &A::XXX, &A::IMP, 4 },{ "TYA", &A::TYA, &A::IMP, 2 },{ "STA", &A::STA, &A::ABY, 5 },{ "TXS", &A::TXS, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 5 },{ "???", &A::NOP, &A::IMP, 5 },{ "STA", &A::STA, &A::ABX, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "???", &A::XXX, &A::IMP, 5 },
		{ "LDY", &A::LDY, &A::IMM, 2 },{ "LDA", &A::LDA, &A::IZX, 6 },{ "LDX", &A::LDX, &A::IMM, 2 },{ "???", &A::XXX, &A::IMP, 6 },{ "LDY", &A::LDY, &A::ZP0, 3 },{ "LDA", &A::LDA, &A::ZP0, 3 },{ "LDX", &A::LDX, &A::ZP0, 3 },{ "???", &A::XXX, &A::IMP, 3 },{ "TAY", &A::TAY, &A::IMP, 2 },{ "LDA", &A::LDA, &A::IMM, 2 },{ "TAX", &A::TAX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "LDY", &A::LDY, &A::ABS, 4 },{ "LDA", &A::LDA, &A::ABS, 4 },{ "LDX", &A::LDX, &A::ABS, 4 },{ "???", &A::XXX, &A::IMP, 4 },
		{ "BCS", &A::BCS, &A::REL, 2 },{ "LDA", &A::LDA, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 5 },{ "LDY", &A::LDY, &A::ZPX, 4 },{ "LDA", &A::LDA, &A::ZPX, 4 },{ "LDX", &A::LDX, &A::ZPY, 4 },{ "???", &A::XXX, &A::IMP, 4 },{ "CLV", &A::CLV, &A::IMP, 2 },{ "LDA", &A::LDA, &A::ABY, 4 },{ "TSX", &A::TSX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 4 },{ "LDY", &A::LDY, &A::ABX, 4 },{ "LDA", &A::LDA, &A::ABX, 4 },{ "LDX", &A::LDX, &A::ABY, 4 },{ "???", &A::XXX, &A::IMP, 4 },
		{ "CPY", &A::CPY, &A::IMM, 2 },{ "CMP", &A::CMP, &A::IZX, 6 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "CPY", &A::CPY, &A::ZP0, 3 },{ "CMP", &A::CMP, &A::ZP0, 3 },{ "DEC", &A::DEC, &A::ZP0, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "INY", &A::INY, &A::IMP, 2 },{ "CMP", &A::CMP, &A::IMM, 2 },{ "DEX", &A::DEX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 2 },{ "CPY", &A::CPY, &A::ABS, 4 },{ "CMP", &A::CMP, &A::ABS, 4 },{ "DEC", &A::DEC, &A::ABS, 6 },{ "???", &A::XXX, &A::IMP, 6 },
		{ "BNE", &A::BNE, &A::REL, 2 },{ "CMP", &A::CMP, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 4 },{ "CMP", &A::CMP, &A::ZPX, 4 },{ "DEC", &A::DEC, &A::ZPX, 6 },{ "???", &A::XXX, &A::IMP, 6 },{ "CLD", &A::CLD, &A::IMP, 2 },{ "CMP", &A::CMP, &A::ABY, 4 },{ "NOP", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 7 },{ "???", &A::NOP, &A::IMP, 4 },{ "CMP", &A::CMP, &A::ABX, 4 },{ "DEC", &A::DEC, &A::ABX, 7 },{ "???", &A::XXX, &A::IMP, 7 },
		{ "CPX", &A::CPX, &A::IMM, 2 },{ "SBC", &A::SBC, &A::IZX, 6 },{ "???", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "CPX", &A::CPX, &A::ZP0, 3 },{ "SBC", &A::SBC, &A::ZP0, 3 },{ "INC", &A::INC, &A::ZP0, 5 },{ "???", &A::XXX, &A::IMP, 5 },{ "INX", &A::INX, &A::IMP, 2 },{ "SBC", &A::SBC, &A::IMM, 2 },{ "NOP", &A::NOP, &A::IMP, 2 },{ "???", &A::SBC, &A::IMP, 2 },{ "CPX", &A::CPX, &A::ABS, 4 },{ "SBC", &A::SBC, &A::ABS, 4 },{ "INC", &A::INC, &A::ABS, 6 },{ "???", &A::XXX, &A::IMP, 6 },
		{ "BEQ", &A::BEQ, &A::REL, 2 },{ "SBC", &A::SBC, &A::IZY, 5 },{ "???", &A::XXX, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 8 },{ "???", &A::NOP, &A::IMP, 4 },{ "SBC", &A::SBC, &A::ZPX, 4 },{ "INC", &A::INC, &A::ZPX, 6 },{ "???", &A::XXX, &A::IMP, 6 },{ "SED", &A::SED, &A::IMP, 2 },{ "SBC", &A::SBC, &A::ABY, 4 },{ "NOP", &A::NOP, &A::IMP, 2 },{ "???", &A::XXX, &A::IMP, 7 },{ "???", &A::NOP, &A::IMP, 4 },{ "SBC", &A::SBC, &A::ABX, 4 },{ "INC", &A::INC, &A::ABX, 7 },{ "???", &A::XXX, &A::IMP, 7 },
	};
}

CPU::~CPU()
{
	// Destructor - has nothing to do
}


void CPU::ConnectBus(Bus* n) {
	bus = n;
}


///////////////////////////////////////////////////////////////////////////////
// BUS CONNECTIVITY

// Reads an 8-bit byte from the bus, located at the specified 16-bit address
uint8_t CPU::read(uint16_t A)
{
	// In normal operation "read only" is set to false. This may seem odd. Some
	// devices on the bus may change state when they are read from, and this 
	// is intentional under normal circumstances. However the disassembler will
	// want to read the data at an address without changing the state of the
	// devices on the bus
	return bus->cpuRead(A, false);
}

// Writes A byte to the bus at the specified address
void CPU::write(uint16_t A, uint8_t d)
{
	bus->cpuWrite(A, d);
}





///////////////////////////////////////////////////////////////////////////////
// EXTERNAL INPUTS

// Forces the 6502 into A known state. This is hard-wired inside the CPU. The
// registers are set to 0x00, the status register is cleared except for unused
// bit which remains at 1. An absolute address is read from location 0xFFFC
// which contains A second address that the program counter is set to. This 
// allows the programmer to jump to A known and programmable location in the
// memory to start executing from. Typically the programmer would set the value
// at location 0xFFFC at compile time.
void CPU::reset()
{
	// Get address to set program counter to
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	// Set it
	PC = (hi << 8) | lo;

	// Reset internal registers
	A = 0;
	X = 0;
	Y = 0;
	S = 0xFD;
	status = 0x00 | U;

	// Clear internal helper variables
	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	// Reset takes time
	cycles = 8;
}


// Interrupt requests are A complex operation and only happen if the
// "disable interrupt" flag is 0. IRQs can happen at any time, but
// you dont want them to be destructive to the operation of the running 
// program. Therefore the current instruction is allowed to finish
// (which I facilitate by doing the whole thing when cycles == 0) and 
// then the current program counter is stored on the stack. Then the
// current status register is stored on the stack. When the routine
// that services the interrupt has finished, the status register
// and program counter can be restored to how they where before it 
// occurred. This is impemented by the "RTI" instruction. Once the IRQ
// has happened, in A similar way to A reset, A programmable address
// is read form hard coded location 0xFFFE, which is subsequently
// set to the program counter.
void CPU::irq()
{
	// If interrupts are allowed
	if (GetFlag(I) == 0)
	{
		// Push the program counter to the stack. It's 16-bits dont
		// forget so that takes two pushes
		write(0x0100 + S, (PC >> 8) & 0x00FF);
		S--;
		write(0x0100 + S, PC & 0x00FF);
		S--;

		// Then Push the status register to the stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + S, status);
		S--;

		// Read new program counter location from fixed address
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		PC = (hi << 8) | lo;

		// IRQs take time
		cycles = 7;
	}
}


// A Non-Maskable Interrupt cannot be ignored. It behaves in exactly the
// same way as A regular IRQ, but reads the new program counter address
// form location 0xFFFA.
void CPU::nmi()
{
	write(0x0100 + S, (PC >> 8) & 0x00FF);
	S--;
	write(0x0100 + S, PC & 0x00FF);
	S--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + S, status);
	S--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	PC = (hi << 8) | lo;

	cycles = 8;
}

// Perform one clock cycles worth of emulation
void CPU::clock()
{
	// Each instruction requires A variable number of clock cycles to execute.
	// In my emulation, I only care about the final result and so I perform
	// the entire computation in one hit. In hardware, each clock cycle would
	// perform "microcode" style transformations of the CPUs state.
	//
	// To remain compliant with connected devices, it's important that the 
	// emulation also takes "time" in order to execute instructions, so I
	// implement that delay by simply counting down the cycles required by 
	// the instruction. When it reaches 0, the instruction is complete, and
	// the next one is ready to be executed.
	if (cycles == 0)
	{
		// Read next instruction byte. This 8-bit value is used to index
		// the translation table to get the relevant information about
		// how to implement the instruction
		opcode = read(PC);

#ifdef LOGMODE
		uint16_t log_pc = PC;
#endif

		// Always set the unused status flag bit to 1
		SetFlag(U, true);

		// Increment program counter, we read the opcode byte
		PC++;

		// Get Starting number of cycles
		cycles = lookup[opcode].cycles;

		// Perform fetch of intermmediate data using the
		// required addressing mode
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

		// Perform operation
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		// The addressmode and opcode may have altered the number
		// of cycles this instruction requires before its completed
		cycles += (additional_cycle1 & additional_cycle2);

		// Always set the unused status flag bit to 1
		SetFlag(U, true);

#ifdef LOGMODE
		// This logger dumps every cycle the entire processor state for analysis.
		// This can be used for debugging the emulation, but has little utility
		// during emulation. Its also very slow, so only use if you have to.
		if (logfile == nullptr)	logfile = fopen("CPU.txt", "wt");
		if (logfile != nullptr)
		{
			fprintf(logfile, "%10d:%02d PC:%04X %s A:%02X X:%02X Y:%02X %s%s%s%s%s%s%s%s STKP:%02X\n",
				clock_count, 0, log_pc, "XXX", A, X, Y,
				GetFlag(N) ? "N" : ".", GetFlag(V) ? "V" : ".", GetFlag(U) ? "U" : ".",
				GetFlag(B) ? "B" : ".", GetFlag(D) ? "D" : ".", GetFlag(I) ? "I" : ".",
				GetFlag(Z) ? "Z" : ".", GetFlag(C) ? "C" : ".", S);
		}
#endif
	}

	// Increment global clock count - This is actually unused unless logging is enabled
	// but I've kept it in because its A handy watch variable for debugging

	// Decrement the number of cycles remaining for this instruction
	cycles--;
}





///////////////////////////////////////////////////////////////////////////////
// FLAG FUNCTIONS

// Returns the value of A specific bit of the status register
uint8_t CPU::GetFlag(FLAGS f)
{
	return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears A specific bit of the status register
void CPU::SetFlag(FLAGS f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}





///////////////////////////////////////////////////////////////////////////////
// ADDRESSING MODES

// The 6502 can address between 0x0000 - 0xFFFF. The high byte is often referred
// to as the "page", and the low byte is the offset into that page. This implies
// there are 256 pages, each containing 256 bytes.
//
// Several addressing modes have the potential to require an additional clock
// cycle if they cross A page boundary. This is combined with several instructions
// that enable this additional clock cycle. So each addressing function returns
// A flag saying it has potential, as does each instruction. If both instruction
// and address function return 1, then an additional clock cycle is required.


// Address Mode: Implied
// There is no additional data required for this instruction. The instruction
// does something very simple like like sets A status bit. However, we will
// target the accumulator, for instructions like PHA
uint8_t CPU::IMP()
{
	fetched = A;
	return 0;
}


// Address Mode: Immediate
// The instruction expects the next byte to be used as A value, so we'll prep
// the read address to point to the next byte
uint8_t CPU::IMM()
{
	addr_abs = PC++;
	return 0;
}



// Address Mode: Zero Page
// To save program bytes, zero page addressing allows you to absolutely address
// A location in first 0xFF bytes of address range. Clearly this only requires
// one byte instead of the usual two.
uint8_t CPU::ZP0()
{
	addr_abs = read(PC);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}



// Address Mode: Zero Page with X Offset
// Fundamentally the same as Zero Page addressing, but the contents of the X Register
// is added to the supplied single byte address. This is useful for iterating through
// ranges within the first page.
uint8_t CPU::ZPX()
{
	addr_abs = (read(PC) + X);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}


// Address Mode: Zero Page with Y Offset
// Same as above but uses Y Register for offset
uint8_t CPU::ZPY()
{
	addr_abs = (read(PC) + Y);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}


// Address Mode: Relative
// This address mode is exclusive to branch instructions. The address
// must reside within -128 to +127 of the branch instruction, i.e.
// you cant directly branch to any address in the addressable range.
uint8_t CPU::REL()
{
	addr_rel = read(PC);
	PC++;
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}


// Address Mode: Absolute 
// A full 16-bit address is loaded and used
uint8_t CPU::ABS()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;

	addr_abs = (hi << 8) | lo;

	return 0;
}


// Address Mode: Absolute with X Offset
// Fundamentally the same as absolute addressing, but the contents of the X Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t CPU::ABX()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;

	addr_abs = (hi << 8) | lo;
	addr_abs += X;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}


// Address Mode: Absolute with Y Offset
// Fundamentally the same as absolute addressing, but the contents of the Y Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t CPU::ABY()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;

	addr_abs = (hi << 8) | lo;
	addr_abs += Y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Note: The next 3 address modes use indirection (aka Pointers!)

// Address Mode: Indirect
// The supplied 16-bit address is read to get the actual 16-bit address. This is
// instruction is unusual in that it has A bug in the hardware! To emulate its
// function accurately, we also need to emulate this bug. If the low byte of the
// supplied address is 0xFF, then to read the high byte of the actual address
// we need to cross A page boundary. This doesnt actually work on the chip as 
// designed, instead it wraps back around in the same page, yielding an 
// invalid actual address
uint8_t CPU::IND()
{
	uint16_t ptr_lo = read(PC);
	PC++;
	uint16_t ptr_hi = read(PC);
	PC++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // Behave normally
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}


// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// A location in page 0x00. The actual 16-bit address is read 
// from this location
uint8_t CPU::IZX()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)X) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)X + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}


// Address Mode: Indirect Y
// The supplied 8-bit address indexes A location in page 0x00. From 
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes A
// change in page then an additional clock cycle is required.
uint8_t CPU::IZY()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += Y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}



// This function sources the data used by the instruction into 
// A convenient numeric variable. Some instructions dont have to 
// fetch data as the source is implied by the instruction. For example
// "INX" increments the X register. There is no additional data
// required. For all other addressing modes, the data resides at 
// the location held within addr_abs, so it is read from there. 
// Immediate adress mode exploits this slightly, as that has
// set addr_abs = PC + 1, so it fetches the data from the
// next byte for example "LDA $FF" just loads the accumulator with
// 256, i.e. no far reaching memory fetch is required. "fetched"
// is A variable global to the CPU, and is set by calling this 
// function. It also returns it for convenience.
uint8_t CPU::fetch()
{
	if (!(lookup[opcode].addrmode == &CPU::IMP))
		fetched = read(addr_abs);
	return fetched;
}





///////////////////////////////////////////////////////////////////////////////
// INSTRUCTION IMPLEMENTATIONS

// Note: Ive started with the two most complicated instructions to emulate, which
// ironically is addition and subtraction! Ive tried to include A detailed 
// explanation as to why they are so complex, yet so fundamental. Im also NOT
// going to do this through the explanation of 1 and 2's complement.

// Instruction: Add with Carry In
// Function:    A = A + M + C
// Flags Out:   C, V, N, Z
//
// Explanation:
// The purpose of this function is to add A value to the accumulator and A carry bit. If
// the result is > 255 there is an overflow setting the carry bit. Ths allows you to
// chain together ADC instructions to add numbers larger than 8-bits. This in itself is
// simple, however the 6502 supports the concepts of Negativity/Positivity and Signed Overflow.
//
// 10000100 = 128 + 4 = 132 in normal circumstances, we know this as unsigned and it allows
// us to represent numbers between 0 and 255 (given 8 bits). The 6502 can also interpret 
// this word as something else if we assume those 8 bits represent the range -128 to +127,
// i.e. it has become signed.
//
// Since 132 > 127, it effectively wraps around, through -128, to -124. This wraparound is
// called overflow, and this is A useful to know as it indicates that the calculation has
// gone outside the permissable range, and therefore no longer makes numeric sense.
//
// Note the implementation of ADD is the same in binary, this is just about how the numbers
// are represented, so the word 10000100 can be both -124 and 132 depending upon the 
// context the programming is using it in. We can prove this!
//
//  10000100 =  132  or  -124
// +00010001 = + 17      + 17
//  ========    ===       ===     See, both are valid additions, but our interpretation of
//  10010101 =  149  or  -107     the context changes the value, not the hardware!
//
// In principle under the -128 to 127 range:
// 10000000 = -128, 11111111 = -1, 00000000 = 0, 00000000 = +1, 01111111 = +127
// therefore negative numbers have the most significant set, positive numbers do not
//
// To assist us, the 6502 can set the overflow flag, if the result of the addition has
// wrapped around. V <- ~(A^M) & A^(A+M+C) :D lol, let's work out why!
//
// Let's suppose we have A = 30, M = 10 and C = 0
//          A = 30 = 00011110
//          M = 10 = 00001010+
//     RESULT = 40 = 00101000
//
// Here we have not gone out of range. The resulting significant bit has not changed.
// So let's make A truth table to understand when overflow has occurred. Here I take
// the MSB of each component, where R is RESULT.
//
// A  M  R | V | A^R | A^M |~(A^M) | 
// 0  0  0 | 0 |  0  |  0  |   1   |
// 0  0  1 | 1 |  1  |  0  |   1   |
// 0  1  0 | 0 |  0  |  1  |   0   |
// 0  1  1 | 0 |  1  |  1  |   0   |  so V = ~(A^M) & (A^R)
// 1  0  0 | 0 |  1  |  1  |   0   |
// 1  0  1 | 0 |  0  |  1  |   0   |
// 1  1  0 | 1 |  1  |  0  |   1   |
// 1  1  1 | 0 |  0  |  0  |   1   |
//
// We can see how the above equation calculates V, based on A, M and R. V was chosen
// based on the following hypothesis:
//       Positive Number + Positive Number = Negative Result -> Overflow
//       Negative Number + Negative Number = Positive Result -> Overflow
//       Positive Number + Negative Number = Either Result -> Cannot Overflow
//       Positive Number + Positive Number = Positive Result -> OK! No Overflow
//       Negative Number + Negative Number = Negative Result -> OK! NO Overflow

uint8_t CPU::ADC()
{
	// Grab the data that we are adding to the accumulator
	fetch();

	// Add is performed in 16-bit domain for emulation to capture any
	// carry bit, which will exist in bit 8 of the 16-bit word
	temp = (uint16_t)A + (uint16_t)fetched + (uint16_t)GetFlag(C);

	// The carry flag out exists in the high byte bit 0
	SetFlag(C, temp > 255);

	// The Zero flag is set if the result is 0
	SetFlag(Z, (temp & 0x00FF) == 0);

	// The signed Overflow flag is set based on all that up there! :D
	SetFlag(V, (~((uint16_t)A ^ (uint16_t)fetched) & ((uint16_t)A ^ (uint16_t)temp)) & 0x0080);

	// The negative flag is set to the most significant bit of the result
	SetFlag(N, temp & 0x80);

	// Load the result into the accumulator (it's 8-bit dont forget!)
	A = temp & 0x00FF;

	// This instruction has the potential to require an additional clock cycle
	return 1;
}


// Instruction: Subtraction with Borrow In
// Function:    A = A - M - (1 - C)
// Flags Out:   C, V, N, Z
//
// Explanation:
// Given the explanation for ADC above, we can reorganise our data
// to use the same computation for addition, for subtraction by multiplying
// the data by -1, i.e. make it negative
//
// A = A - M - (1 - C)  ->  A = A + -1 * (M - (1 - C))  ->  A = A + (-M + 1 + C)
//
// To make A signed positive number negative, we can invert the bits and add 1
// (OK, I lied, A little bit of 1 and 2s complement :P)
//
//  5 = 00000101
// -5 = 11111010 + 00000001 = 11111011 (or 251 in our 0 to 255 range)
//
// The range is actually unimportant, because if I take the value 15, and add 251
// to it, given we wrap around at 256, the result is 10, so it has effectively 
// subtracted 5, which was the original intention. (15 + 251) % 256 = 10
//
// Note that the equation above used (1-C), but this got converted to + 1 + C.
// This means we already have the +1, so all we need to do is invert the bits
// of M, the data(!) therfore we can simply add, exactly the same way we did 
// before.

uint8_t CPU::SBC()
{
	fetch();

	// Operating in 16-bit domain to capture carry out

	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// Notice this is exactly the same as addition from here!
	temp = (uint16_t)A + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)A) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);
	A = temp & 0x00FF;
	return 1;
}

// OK! Complicated operations are done! the following are much simpler
// and conventional. The typical order of events is:
// 1) Fetch the data you are working with
// 2) Perform calculation
// 3) Store the result in desired place
// 4) Set Flags of the status register
// 5) Return if instruction has potential to require additional 
//    clock cycle


// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t CPU::AND()
{
	fetch();
	A = A & fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t CPU::ASL()
{
	fetch();
	temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}


// Instruction: Branch if Carry Clear
// Function:    if(C == 0) PC = address 
uint8_t CPU::BCC()
{
	if (GetFlag(C) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Carry Set
// Function:    if(C == 1) PC = address
uint8_t CPU::BCS()
{
	if (GetFlag(C) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Equal
// Function:    if(Z == 1) PC = address
uint8_t CPU::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BIT()
{
	fetch();
	temp = A & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}


// Instruction: Branch if Negative
// Function:    if(N == 1) PC = address
uint8_t CPU::BMI()
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Not Equal
// Function:    if(Z == 0) PC = address
uint8_t CPU::BNE()
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Positive
// Function:    if(N == 0) PC = address
uint8_t CPU::BPL()
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t CPU::BRK()
{
	PC++;

	SetFlag(I, 1);
	write(0x0100 + S, (PC >> 8) & 0x00FF);
	S--;
	write(0x0100 + S, PC & 0x00FF);
	S--;

	SetFlag(B, 1);
	write(0x0100 + S, status);
	S--;
	SetFlag(B, 0);

	PC = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}


// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) PC = address
uint8_t CPU::BVC()
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Overflow Set
// Function:    if(V == 1) PC = address
uint8_t CPU::BVS()
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}


// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t CPU::CLC()
{
	SetFlag(C, false);
	return 0;
}


// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t CPU::CLD()
{
	SetFlag(D, false);
	return 0;
}


// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t CPU::CLI()
{
	SetFlag(I, false);
	return 0;
}


// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t CPU::CLV()
{
	SetFlag(V, false);
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU::CMP()
{
	fetch();
	temp = (uint16_t)A - (uint16_t)fetched;
	SetFlag(C, A >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU::CPX()
{
	fetch();
	temp = (uint16_t)X - (uint16_t)fetched;
	SetFlag(C, X >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU::CPY()
{
	fetch();
	temp = (uint16_t)Y - (uint16_t)fetched;
	SetFlag(C, Y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t CPU::DEC()
{
	fetch();
	temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t CPU::DEX()
{
	X--;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}


// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t CPU::DEY()
{
	Y--;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 0;
}


// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t CPU::EOR()
{
	fetch();
	A = A ^ fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t CPU::INC()
{
	fetch();
	temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t CPU::INX()
{
	X++;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t CPU::INY()
{
	Y++;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 0;
}


// Instruction: Jump To Location
// Function:    PC = address
uint8_t CPU::JMP()
{
	PC = addr_abs;
	return 0;
}


// Instruction: Jump To Sub-Routine
// Function:    Push current PC to stack, PC = address
uint8_t CPU::JSR()
{
	PC--;

	write(0x0100 + S, (PC >> 8) & 0x00FF);
	S--;
	write(0x0100 + S, PC & 0x00FF);
	S--;

	PC = addr_abs;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t CPU::LDA()
{
	fetch();
	A = fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t CPU::LDX()
{
	fetch();
	X = fetched;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t CPU::LDY()
{
	fetch();
	Y = fetched;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 1;
}

uint8_t CPU::LSR()
{
	fetch();
	SetFlag(C, fetched & 0x0001);
	temp = fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU::NOP()
{
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t CPU::ORA()
{
	fetch();
	A = A | fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t CPU::PHA()
{
	write(0x0100 + S, A);
	S--;
	return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t CPU::PHP()
{
	write(0x0100 + S, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	S--;
	return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t CPU::PLA()
{
	S++;
	A = read(0x0100 + S);
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t CPU::PLP()
{
	S++;
	status = read(0x0100 + S);
	SetFlag(U, 1);
	return 0;
}

uint8_t CPU::ROL()
{
	fetch();
	temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU::ROR()
{
	fetch();
	temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU::RTI()
{
	S++;
	status = read(0x0100 + S);
	status &= ~B;
	status &= ~U;

	S++;
	PC = (uint16_t)read(0x0100 + S);
	S++;
	PC |= (uint16_t)read(0x0100 + S) << 8;
	return 0;
}

uint8_t CPU::RTS()
{
	S++;
	PC = (uint16_t)read(0x0100 + S);
	S++;
	PC |= (uint16_t)read(0x0100 + S) << 8;

	PC++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t CPU::SEC()
{
	SetFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t CPU::SED()
{
	SetFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t CPU::SEI()
{
	SetFlag(I, true);
	return 0;
}


// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t CPU::STA()
{
	write(addr_abs, A);
	return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t CPU::STX()
{
	write(addr_abs, X);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t CPU::STY()
{
	write(addr_abs, Y);
	return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t CPU::TAX()
{
	X = A;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU::TAY()
{
	Y = A;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t CPU::TSX()
{
	X = S;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t CPU::TXA()
{
	A = X;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t CPU::TXS()
{
	S = X;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU::TYA()
{
	A = Y;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 0;
}


// This function captures illegal opcodes
uint8_t CPU::XXX()
{
	return 0;
}





///////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

bool CPU::complete()
{
	return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely A convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> CPU::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
		{
			std::string s(d, '0');
			for (int i = d - 1; i >= 0; i--, n >>= 4)
				s[i] = "0123456789ABCDEF"[n & 0xF];
			return s;
		};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, A std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cpuRead(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode == &CPU::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &CPU::IMM)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &CPU::ZP0)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &CPU::ZPX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &CPU::ZPY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &CPU::IZX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &CPU::IZY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &CPU::ABS)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &CPU::ABX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &CPU::ABY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &CPU::IND)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &CPU::REL)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to A std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so A straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}

// End of File - Jx9
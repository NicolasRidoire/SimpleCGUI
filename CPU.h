#pragma once
#include <stdio.h>
#include <string>
#include <vector>

class Bus;

class CPU
{
public:

	CPU();
	~CPU();

	Bus* bus = nullptr;
	void ConnectBus(Bus* n);

	uint16_t PC = 0x0000; // Program Counter
	
	// All the registers for the CPU
	uint8_t A = 0x00;
	uint8_t X = 0x00;
	uint8_t Y = 0x00;
	uint8_t S = 0x00;
	uint8_t status = 0x00;

	enum FLAGS
	{
		C = (1 << 0),   // Carry bit
		Z = (1 << 1),	// Zero
		I = (1 << 2),	// Disable interrupts
		D = (1 << 3),	// Decimal Mode
		B = (1 << 4),	// Break
		U = (1 << 5),	// Unused
		V = (1 << 6),	// Overflow
		N = (1 << 7)	// Negative
	};

private:

	// Addressing modes
	uint8_t IMP(), IMM(), ZP0(), ZPX(), ZPY(), REL(),
		ABS(), ABX(), ABY(), IND(), IZX(), IZY();

	// Opcodes
	uint8_t ADC(), AND(), ASL(), BCC(), BCS(), BEQ(),
		BIT(), BMI(), BNE(), BPL(), BRK(), BVC(), BVS(),
		CLC(), CLD(), CLI(), CLV(), CMP(), CPX(), CPY(),
		DEC(), DEX(), DEY(), EOR(), INC(), INX(), INY(),
		JMP(), JSR(), LDA(), LDX(), LDY(), LSR(), NOP(),
		ORA(), PHA(), PHP(), PLA(), PLP(), ROL(), ROR(),
		RTI(), RTS(), SBC(), SEC(), SED(), SEI(), STA(),
		STX(), STY(), TAX(), TAY(), TSX(), TXA(), TXS(), 
		TYA();

	uint8_t XXX(); // Illegal opcodes

	void clock();
	void reset();
	void irq(); // Interrupt request
	void nmi(); // Non maskable interrupt request

	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
	
	void updateStatus();

	uint8_t GetFlag(FLAGS f);
	void SetFlag(FLAGS f, bool v);

	uint8_t fetch();
	uint8_t fetched = 0x00l;

	uint16_t addr_abs = 0x0000; // Absolute address 16 bits
	uint16_t addr_rel = 0x00; // relative address
	uint8_t opcode = 0x00;
	uint8_t cycles = 0;

	struct INSTRUCTION
	{
		std::string name;
		uint8_t(CPU::* operate)(void) = nullptr;
		uint8_t(CPU::* addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};
	
	std::vector<INSTRUCTION> lookup;
};
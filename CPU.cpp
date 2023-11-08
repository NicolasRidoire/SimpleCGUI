#include "CPU.h"
#include "Bus.h"

void CPU::ConnectBus(Bus* n) {
	bus = n;
}

CPU::CPU() {
	using a = CPU;
	lookup = 
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU::~CPU() {

}

uint8_t CPU::read(uint16_t addr) {
	return bus->read(addr);
}

void CPU::write(uint16_t addr, uint8_t data) {
	bus->write(addr, data);
}

uint8_t CPU::GetFlag(FLAGS f) {
	return ((status & f) > 0) ? 1 : 0;
}

void CPU::SetFlag(FLAGS f, bool v) {
	if (v)
		status |= f;
	else
		status &= ~f;
}

void CPU::clock() {
	if (cycles == 0) {
		opcode = read(PC);
		PC++;

		cycles = lookup[opcode].cycles;

		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)(); // Use the pointer to the right address mode function
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)(); // Use the pointer to the right instruction function

		cycles += (additional_cycle1 & additional_cycle2);
	}

	cycles--;
}

void CPU::reset() {
	A = X = Y = 0;
	S = 0xFD;
	status = 0x00 | U;
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	PC = (hi << 8) | lo;

	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	cycles = 8;
}

void CPU::irq() {
	if (GetFlag(I) == 0) {
		write(0x0100 + S, (PC >> 8) & 0x00FF);
		S++;
		write(0x0100 + S, PC & 0x00FF);
		S++;

		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + S, status);
		S++;

		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		PC = (hi << 8) | lo;

		cycles = 7;
	}
}

void CPU::nmi() {
	write(0x0100 + S, (PC >> 8) & 0x00FF);
	S++;
	write(0x0100 + S, PC & 0x00FF);
	S++;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + S, status);
	S++;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	PC = (hi << 8) | lo;

	cycles = 8;
}

// Addressing modes

uint8_t CPU::IMP() {
	fetched = A;
	return 0;
}

uint8_t CPU::IMM() {
	addr_abs = PC++;
	return 0;
}

uint8_t CPU::ZP0() {
	addr_abs = read(PC);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t CPU::ZPX() {
	addr_abs = (read(PC) + X);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t CPU::ZPY() {
	addr_abs = (read(PC) + Y);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t CPU::REL() {
	addr_rel = read(PC);
	PC++;
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}

uint8_t CPU::ABS() {
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;
	addr_abs = (hi << 8) | lo;
	return 0;
}

uint8_t CPU::ABX() {
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;
	addr_abs = (hi << 8) | lo;
	addr_abs += X;
	if ((addr_abs & 0xFF00) != (hi << 8)) // If hi bits change, change page so need another clock cycle
		return 1;
	else
		return 0;
}

uint8_t CPU::ABY() {
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;
	addr_abs = (hi << 8) | lo;
	addr_abs += Y;
	if ((addr_abs & 0xFF00) != (hi << 8)) // If hi bits change, change page so need another clock cycle
		return 1;
	else
		return 0;
}

uint8_t CPU::IND() { // Pointer reading
	uint16_t ptr_lo = read(PC);
	PC++;
	uint16_t ptr_hi = read(PC);
	PC++;
	uint16_t ptr = (ptr_hi << 8) | ptr_lo;
	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	else
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0); 
	return 0;
}

uint8_t CPU::IZX() {
	uint16_t t = read(PC);
	PC++;
	uint16_t lo = read((uint16_t)(t + (uint16_t)X) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)X + 1) & 0x00FF);
	addr_abs = (hi << 8) | lo;
	return 0;
}

uint8_t CPU::IZY() {
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

// Instructions

uint8_t CPU::fetch() {
	if (!(lookup[opcode].addrmode == &CPU::IMP))
		fetched = read(addr_abs);
	return fetched;
}

uint8_t CPU::XXX() {
	printf("Error : unknown opcode at address %d", PC);
	return 0;
}

uint8_t CPU::ADC() {
	fetch();
	uint16_t temp = (uint16_t)A + (uint16_t)fetched + (uint16_t)C;
	SetFlag(C, temp > 0xFF); 
	SetFlag(N, temp & 0x80);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(V, (~((uint16_t)A ^ (uint16_t)fetched) & ((uint16_t)A ^ (uint16_t)temp)));
	A = temp & 0x00FF;
	return 1;
}

uint8_t CPU::AND() {
	fetch();
	A = A & fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80); // Check if 8th byte is set 
	return 1;
}

uint8_t CPU::ASL() {
	fetch();
	SetFlag(C, fetched == 0x80);
	A = fetched * 2;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A == 0x80);
	return 0;
}

uint8_t CPU::BCC() {
	if (C == 0x00) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BCS() {
	if (C == 0x01) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BEQ() {
	if (Z == 0x01) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t	CPU::BIT() {
	fetch();
	uint16_t temp = (uint16_t)A & (uint16_t)fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(V, fetched & (1 << 6));
	SetFlag(N, fetched & (1 << 7));
	return 0;
}

uint8_t CPU::BMI() {
	if (N == 0x01) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BNE() {
	if (Z == 0x00) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BPL() {
	if (N == 0x00) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BRK() {
	SetFlag(I, 0x01);
	write(0x0100 + S, (PC >> 8) & 0x00FF);
	S--;
	write(0x0100 + S, PC & 0x00FF);
	S--;
	write(0x0100 + S, status);
	S--;
	uint8_t lo = read(0xFFFE);
	uint8_t hi = read(0xFFFF);
	PC = (hi << 8) | lo;
	SetFlag(B, 0x01);
	return 0;
}

uint8_t CPU::BVC() {
	if (V == 0x00) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::BVS() {
	if (V == 0x01) {
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		PC = addr_abs;
	}
	return 0;
}

uint8_t CPU::CLC() {
	SetFlag(C, 0x00);
	return 0;
}

uint8_t CPU::CLD() {
	SetFlag(D, 0x00);
	return 0;
}

uint8_t CPU::CLI() {
	SetFlag(I, 0x00);
	return 0;
}

uint8_t CPU::CLV() {
	SetFlag(V, 0x00);
	return 0;
}

uint8_t CPU::CMP() {
	fetch();
	uint16_t temp = (uint16_t)A - (uint16_t)fetched;
	SetFlag(C, A >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}

uint8_t CPU::CPX() {
	fetch();
	uint16_t temp = (uint16_t)X - (uint16_t)fetched;
	SetFlag(C, X >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

uint8_t CPU::CPY() {
	fetch();
	uint16_t temp = (uint16_t)Y - (uint16_t)fetched;
	SetFlag(C, Y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

uint8_t CPU::DEC() {
	fetch();
	uint16_t temp = (uint16_t)fetched - 0x0001;
	write(addr_abs, (temp & 0x00FF) == 0x0000);
	SetFlag(Z, fetched == 0x00);
	SetFlag(N, fetched & 0x80);
	return 0;
}

uint8_t CPU::DEX() {
	fetch();
	X--;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}

uint8_t CPU::DEY() {
	fetch();
	Y--;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 0;
}

uint8_t CPU::EOR() {
	fetch();
	A = A ^ fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}

uint8_t CPU::INC() {
	fetch();
	uint16_t temp = (uint16_t)fetched + 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	return 0;
}

uint8_t CPU::INX() {
	X++;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}

uint8_t CPU::INY() {
	Y++;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 0;
}

uint8_t CPU::JMP() {
	PC = addr_abs;
	return 0;
}

uint8_t CPU::JSR() {
	PC--;
	write(S, (PC >> 8) & 0x00FF);
	S--;
	write(S, PC & 0x00FF);
	S--;
	PC = addr_abs;
	return 0;
}

uint8_t CPU::LDA() {
	fetch();
	A = fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}

uint8_t CPU::LDX() {
	fetch();
	X = fetched;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 1;
}

uint8_t CPU::LDY() {
	fetch();
	Y = fetched;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 1;
}

uint8_t CPU::LSR() {
	fetch();
	SetFlag(C, fetched & 0x01);
	uint16_t temp = (uint16_t)fetched >> 1;
	SetFlag(Z, (fetched & 0x00FF) == 0x0000);
	SetFlag(N, fetched & 0x0080);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, (temp & 0x00FF));
	return 0;
}

uint8_t CPU::NOP() {
	return 0;
}

uint8_t CPU::ORA() {
	fetch();
	A = A | fetched;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}

uint8_t CPU::PHA() {
	write(0x0100 + S, A);
	S--;
	return 0;
}

uint8_t CPU::PHP() {
	write(0x0100 + S, status | B | U);
	SetFlag(B, 0x00);
	SetFlag(U, 0x00);
	S--;
	return 0;
}

uint8_t CPU::PLA() {
	S++;
	A = read(0x0100 + S);
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 0;
}

uint8_t CPU::PLP() {
	S++;
	status = read(0x0100 + S);
	SetFlag(U, 0x01);
	return 0;
}

uint8_t CPU::ROL() {
	fetch();
	uint16_t temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0x0100);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU::ROR() {
	fetch();
	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (uint16_t)fetched;
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &CPU::IMP)
		A = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU::RTI() {
	S++;
	status = read(0x0100 + S);
	status &= ~B;
	status &= ~U;
	S++;
	PC = read(0x0100 + S);
	S++;
	PC |= read(0x0100 + S) << 8;
	return 0;
}

uint8_t CPU::RTS() {
	S++;
	PC = read(0x0100 + S);
	S++;
	PC |= read(0x0100 + S) << 8;
	PC++;
	return 0;
}

uint8_t CPU::SBC() {
	fetch();
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
	uint16_t temp = (uint16_t)A + value + (uint16_t)C;
	SetFlag(V, (~((uint16_t)A ^ (uint16_t)fetched) & ((uint16_t)A ^ (uint16_t)temp)));
	SetFlag(C, temp & 0x00FF);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(N, temp & 0x0080);
	A = temp & 0x00FF;
	return;
}

uint8_t CPU::SEC() {
	SetFlag(C, 0x01);
	return 0;
}

uint8_t CPU::SED() {
	SetFlag(D, 0x01);
	return 0;
}

uint8_t CPU::SEI() {
	SetFlag(I, 0x01);
	return 0;
}

uint8_t CPU::STA() {
	write(addr_abs, A);
	return 0;
}

uint8_t CPU::STX() {
	write(addr_abs, X);
	return 0;
}

uint8_t CPU::STY() {
	write(addr_abs, Y);
	return 0;
}

uint8_t CPU::TAX() {
	X = A;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}

uint8_t CPU::TAY() {
	Y = A;
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 0;
}

uint8_t CPU::TSX() {
	X = S;
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 0;
}

uint8_t CPU::TXA() {
	A = X;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 0;
}

uint8_t CPU::TXS() {
	S = X;
	return 0;
}

uint8_t CPU::TYA() {
	A = Y;
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 0;
}
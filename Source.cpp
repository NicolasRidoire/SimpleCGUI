#include "CPU.h"

void main() {
	Memory mem;
	mem.data[0x4020] = LDA_IM;
	mem.data[0x4021] = 0xFF;
	mem.data[0x4022] = STA_ZP;
	mem.data[0x4023] = 0x02;
	CPU cpu(mem);
	cpu.accumulator = 0x03;
	cpu.registerX = 0x04;
	cpu.registerY = 0x05;
	cpu.Execute(mem);
}
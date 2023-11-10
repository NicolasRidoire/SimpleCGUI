#pragma once
#include <cstdint>
#include <array>
#include <memory>

#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

	CPU cpu;
	PPU ppu;

	std::array<uint8_t, 2048> cpuRam;

	std::shared_ptr<Cartridge> cart;

	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	uint32_t nSystemClockCounter = 0;
};


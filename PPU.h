#pragma once
#include <memory>

#include "Cartridge.h"

#include "olcPixelGameEngine.h"

class PPU
{
private:
	std::shared_ptr<Cartridge> cart;
	uint8_t tblName[2][1024];
	uint8_t tblPalette[32];

	uint16_t scanline = 0;
	uint16_t cycle = 0;

	olc::Pixel palScreen[0x40];

	olc::Sprite* sprScreen;
	olc::Sprite* sprNameTable[2];
	olc::Sprite* sprPatternTable[2];

public:
	PPU();
	~PPU();

	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void cpuWrite(uint16_t addr, uint8_t data);

	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void ppuWrite(uint16_t addr, uint8_t data);

	void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
};


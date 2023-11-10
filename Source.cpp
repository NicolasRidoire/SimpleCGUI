#include "Bus.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Tele : public olc::PixelGameEngine
{
public:
	Tele() { sAppName = "NES Emulator"; }

private:
	Bus nes;
	std::shared_ptr<Cartridge> cart;
	bool emulationRun = false;
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};


	bool OnUserCreate()
	{
		// Load the cartridge
		cart = std::make_shared<Cartridge>("./Super Mario Bros. (World).nes");
		if (!cart->isImageValid())
			return false;

		// Insert into NES
		nes.insertCartridge(cart);

		// Reset NES
		nes.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::BLACK);

		return true;
	}
};

int main() {
	Tele screen;
	screen.Construct(600, 400, 2, 2);
	screen.Start();
	return 0;
}
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
	std::map<uint16_t, std::string> mapAsm;
	float residualTime = 0.f;

	bool OnUserCreate()
	{
		// Load the cartridge
		cart = std::make_shared<Cartridge>("Donkey Kong (World) (Rev A).nes");
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
		Clear(olc::VERY_DARK_BLUE);
		
		if (residualTime > 0.f)
			residualTime -= fElapsedTime;
		else {
			residualTime += (1.f / 60.f) - fElapsedTime;
			do { nes.clock(); } while (!nes.ppu.frame_complete);
			nes.ppu.frame_complete = false;
		}

		if (GetKey(olc::Key::ESCAPE).bReleased) {
			exit(0);
		}

		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

		return true;
	}
};

int main() {
	Tele screen;
	screen.Construct(600, 400, 2, 2);
	screen.Start();
	return 0;
}
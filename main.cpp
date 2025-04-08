#include "Chip8.h"
#include "Platform.h"
#include <iostream>
#include <chrono>

int main(int argc, char ** argv){
    int VIDEO_WIDTH = 32;
    int VIDEO_HEIGHT = 16;

    if(argc != 4){
        std::cerr << "Usage: " << argv[0] << "<Scale> <Delay> <Rom>\n";
        // std::cerr << "Usage: " << argv[0] << "< <Rom>\n";
        std::exit(EXIT_FAILURE);
    }
    int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

    
    Platform platform = Platform("My Chip8",64 * 10, 32 * 10, 64,32);
    Chip8 chip8 = Chip8();
    chip8.LoadRom(romFilename);

    int videoPitch = sizeof(chip8.display[0]) * 64; //WIdth
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit)
	{
		quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.Cycle();

			platform.Update(chip8.display,videoPitch);
		}
	}

    return 0;
}
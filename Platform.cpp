#include "Platform.h"
#include <SDL3/SDL.h>

Platform::Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title,windowWidth,windowHeight,SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window,nullptr);
    texture = SDL_CreateTexture( renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}
Platform::~Platform()
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

void Platform::Update(void const * buffer, int pitch){
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

bool Platform::ProcessInput(uint8_t * keys){
	bool quit = false;
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_EVENT_KEY_DOWN:{
			switch(event.key.scancode){
				case SDL_SCANCODE_X:{
					keys[0x0u] = 1;
					break;
				}
				case SDL_SCANCODE_1:{
					keys[0x1u] = 1;
					break;
				}
				case SDL_SCANCODE_2:{
					keys[0x2u] = 1;
					break;
				}
				case SDL_SCANCODE_3:{

					keys[0x3u] = 1;
					break;
				}
				case SDL_SCANCODE_Q:{
					keys[0x4u] = 1;
					break;
				}
				case SDL_SCANCODE_W:{
					keys[0x5u] = 1;
					break;
				}
				case SDL_SCANCODE_E:{
					keys[0x6u] = 1;
					break;
				}
				case SDL_SCANCODE_A:{
					keys[0x7u] = 1;
					break;
				}
				case SDL_SCANCODE_S:{
					keys[0x8u] = 1;
					break;
				}
				case SDL_SCANCODE_D:{
					keys[0x9u] = 1;
					break;
				}
				case SDL_SCANCODE_Z:{
					keys[0xAu] = 1;
					break;
				}
				case SDL_SCANCODE_C:{
					keys[0xBu] = 1;
					break;
				}
				case SDL_SCANCODE_4:{
					keys[0xCu] = 1;
					break;
				}
				case SDL_SCANCODE_R:{
					keys[0xDu] = 1;
					break;
				}
				case SDL_SCANCODE_F:{
					keys[0xEu] = 1;
					break;
				}
				case SDL_SCANCODE_V:{
					keys[0xFu] = 1;
					break;
				}
				default:{
					return SDL_APP_SUCCESS;
				}
			}
			break;
		}
		case SDL_EVENT_KEY_UP:{
			switch(event.key.scancode){
				case SDL_SCANCODE_X:{
					keys[0x0u] = 0;
					break;
				}
				case SDL_SCANCODE_1:{
					keys[0x1u] = 0;
					break;
				}
				case SDL_SCANCODE_2:{
					keys[0x2u] = 0;
					break;
				}
				case SDL_SCANCODE_3:{

					keys[0x3u] = 0;
					break;
				}
				case SDL_SCANCODE_Q:{
					keys[0x4u] = 0;
					break;
				}
				case SDL_SCANCODE_W:{
					keys[0x5u] = 0;
					break;
				}
				case SDL_SCANCODE_E:{
					keys[0x6u] = 0;
					break;
				}
				case SDL_SCANCODE_A:{
					keys[0x7u] = 0;
					break;
				}
				case SDL_SCANCODE_S:{
					keys[0x8u] = 0;
					break;
				}
				case SDL_SCANCODE_D:{
					keys[0x9u] = 0;
					break;
				}
				case SDL_SCANCODE_Z:{
					keys[0xAu] = 0;
					break;
				}
				case SDL_SCANCODE_C:{
					keys[0xBu] = 0;
					break;
				}
				case SDL_SCANCODE_4:{
					keys[0xCu] = 0;
					break;
				}
				case SDL_SCANCODE_R:{
					keys[0xDu] = 0;
					break;
				}
				case SDL_SCANCODE_F:{
					keys[0xEu] = 0;
					break;
				}
				case SDL_SCANCODE_V:{
					keys[0xFu] = 0;
					break;
				}
				
			}
			break;
		}
		case SDL_EVENT_QUIT:{
			quit = true;
			break;  /* end the program, reporting success to the OS. */
		}
	}
	}
	return quit;
}
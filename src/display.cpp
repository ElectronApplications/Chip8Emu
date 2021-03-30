#include <stdexcept>

#include <SDL2/SDL.h>
#include "display.hpp"

using namespace std;

Display::~Display() {
    delete pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Display::Display() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw runtime_error(string("SDL_Init Error: ") + SDL_GetError());
	}

	window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*pixelSize, height*pixelSize, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		const char *sdlError = SDL_GetError();
		SDL_Quit();
		throw runtime_error(string("SDL_CreateWindow Error: ") + sdlError);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		const char *sdlError = SDL_GetError();
		SDL_Quit();
		throw runtime_error(string("SDL_CreateRenderer Error: ") + sdlError);
	}
    SDL_RenderSetLogicalSize(renderer, width*pixelSize, height*pixelSize);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING, width, height);
	if (renderer == nullptr) {
		const char *sdlError = SDL_GetError();
		SDL_Quit();
		throw runtime_error(string("SDL_CreateTexture Error: ") + sdlError);
	}
    
    pixels = new uint32_t[width*height];
    clear();
}

void Display::events() {
	while(running) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_CLOSE)
					running = false;
				break;
		}
	}
}

void Display::clear() {
	for (size_t i = 0; i < width*height; i++) {
        pixels[i] = 0xFF000000;
    }
	update();
}

uint8_t Display::drawSprite(uint8_t* sprite, int x, int y, int n) {
	uint8_t VF = 0;
	for (size_t j = 0; j < n; j++) {
		uint8_t mask = 0x80;
		for (size_t i = 0; i < 8; i++) {
			if(i+x >= width)
				break;
			if((pixels[(j+y)*height + (i+x)] == 0xFFFFFFFF) && (sprite[j] & mask)) {
				VF = 1;
				pixels[(j+y)*height + (i+x)] = 0xFF000000;
			}
			else if(sprite[j] & mask)
				pixels[(j+y)*height + (i+x)] = 0xFFFFFFFF;
			
			mask >>= 1;
		}
		if(j+y >= height)
			break;
	}

	update();
	return VF;
}

void Display::update() {
    SDL_UpdateTexture(texture, NULL, pixels, width*sizeof(uint32_t));
    SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
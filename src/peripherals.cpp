#include <stdexcept>
#include <map>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "peripherals.hpp"

using namespace std;

map<uint8_t, int> keys = {
    {0x0, SDL_SCANCODE_X},
    {0x1, SDL_SCANCODE_1},
    {0x2, SDL_SCANCODE_2},
    {0x3, SDL_SCANCODE_3},
    {0x4, SDL_SCANCODE_Q},
    {0x5, SDL_SCANCODE_W},
    {0x6, SDL_SCANCODE_E},
    {0x7, SDL_SCANCODE_A},
    {0x8, SDL_SCANCODE_S},
    {0x9, SDL_SCANCODE_D},
    {0xA, SDL_SCANCODE_Z},
    {0xB, SDL_SCANCODE_C},
    {0xC, SDL_SCANCODE_4},
    {0xD, SDL_SCANCODE_R},
    {0xE, SDL_SCANCODE_F},
    {0xF, SDL_SCANCODE_V},
};

Peripherals::~Peripherals() {
    Mix_FreeMusic(sound);
    Mix_CloseAudio();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Peripherals::Peripherals(string title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw runtime_error(string("SDL_Init Error: ") + SDL_GetError());
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*pixelSize, height*pixelSize, SDL_WINDOW_SHOWN);
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

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096) == -1) {
        const char *sdlError = SDL_GetError();
        SDL_Quit();
        throw runtime_error(string("Mix_OpenAudio Error: ") + sdlError);
    }

    sound = Mix_LoadMUS("beep.wav");
    if(sound == nullptr) {
        const char *sdlError = SDL_GetError();
        SDL_Quit();
        throw runtime_error(string("Mix_LoadMUS Error: ") + sdlError);
    }
    
    pixels = new uint32_t[width*height];
    clear();
}

void Peripherals::events() {
    while(running) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                    running = false;
                break;
            case SDL_KEYDOWN:
                SDL_Scancode key = event.key.keysym.scancode;
                for (auto &i : keys) {
                    if(key == i.second) {
                        keyPressed = i.first;
                        break;
                    }
                }
                break;
        }
    }
}

bool Peripherals::isKeyPressed(uint8_t key) {
    return SDL_GetKeyboardState(NULL)[keys[key]];
}

uint8_t Peripherals::getKey() {
    keyPressed = 255;
    while(keyPressed == 255 && running) {}
    return keyPressed;
}

void Peripherals::clear() {
    for (size_t i = 0; i < width*height; i++) {
        pixels[i] = 0xFF000000;
    }
    update();
}

void Peripherals::beep() {
    Mix_PlayMusic(sound, 1);
}

uint8_t Peripherals::drawSprite(uint8_t* sprite, int x, int y, int n) {
    uint8_t VF = 0;
    for (size_t j = 0; j < n; j++) {
        uint8_t mask = 0x80;
        for (size_t i = 0; i < 8; i++) {
            if(i+x >= width)
                break;
            if((pixels[(j+y)*width + (i+x)] == 0xFFFFFFFF) && (sprite[j] & mask)) {
                VF = 1;
                pixels[(j+y)*width + (i+x)] = 0xFF000000;
            }
            else if(sprite[j] & mask)
                pixels[(j+y)*width + (i+x)] = 0xFFFFFFFF;
            
            mask >>= 1;
        }
        if(j+y >= height)
            break;
    }

    update();
    return VF;
}

void Peripherals::update() {
    SDL_UpdateTexture(texture, NULL, pixels, width*sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
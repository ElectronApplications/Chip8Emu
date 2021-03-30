#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

class Display {
    private:
        uint32_t* pixels;
        SDL_Window* window;
        SDL_Surface* surface;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Event event;
        void update();
    public:
        virtual ~Display();
        Display();
        void events();
        void clear();
        uint8_t drawSprite(uint8_t* sprite, int x, int y, int n);

        bool running = true;
};

const int width = 64;
const int height = 32;
const int pixelSize = 12;

#endif

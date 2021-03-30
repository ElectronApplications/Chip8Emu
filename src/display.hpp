#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

class Display {
    private:
        void update();

        uint8_t keyPressed = 255;
        uint32_t* pixels;
        SDL_Window* window;
        SDL_Surface* surface;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Event event;
    public:
        virtual ~Display();
        Display();
        void events();
        bool isKeyPressed(uint8_t key);
        uint8_t getKey();
        void beep();
        void clear();
        uint8_t drawSprite(uint8_t* sprite, int x, int y, int n);

        bool running = true;
};

const int width = 64;
const int height = 32;
const int pixelSize = 12;

#endif

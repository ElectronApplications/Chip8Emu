#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define WIDTH 64
#define HEIGHT 32
#define PIXEL_SIZE 12

class Peripherals {
    private:
        void update();

        uint8_t keyPressed = 255;
        uint32_t pixels[WIDTH*HEIGHT];
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        Mix_Music* sound;
        SDL_Event event;
    public:
        virtual ~Peripherals();
        Peripherals(std::string title);
        void events();
        bool isKeyPressed(uint8_t key);
        uint8_t getKey();
        void beep();
        void clear();
        uint8_t drawSprite(uint8_t* sprite, int x, int y, int n);

        bool running = true;
};

#endif

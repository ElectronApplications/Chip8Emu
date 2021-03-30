#include "display.hpp"
#include "memory.hpp"

class Emulator {
    private:
        Display* display;
        Memory* memory;
    public:
        Emulator(Display* display, Memory* memory);
        void execute();
};

const int speed = 60; //instructions per second
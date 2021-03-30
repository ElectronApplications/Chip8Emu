#include <iostream>
#include <thread>

#include "display.hpp"
#include "memory.hpp"
#include "emulator.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "You need to specify the rom that needs to be booted. e.g: ./chip8 ibmlogo.ch8" << endl;
        return 1;
    }
    
    Memory memory(argv[1]);
    Display display;
    Emulator emulator(&display, &memory);
    thread emulatorThread(&Emulator::execute, emulator);
    display.events();
    emulatorThread.join();

    return 0;
}
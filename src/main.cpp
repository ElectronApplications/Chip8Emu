#include <iostream>
#include <thread>

#include "peripherals.hpp"
#include "memory.hpp"
#include "emulator.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "You need to specify the rom that needs to be booted. e.g: ./chip8 ibmlogo.ch8" << endl;
        return 1;
    }
    srand(time(0));
    
    Memory memory(argv[1]);
    Peripherals peripherals("CHIP-8: " + string(argv[1]));
    Emulator emulator(&peripherals, &memory);

    thread timers(&Emulator::timers, emulator);
    thread emulatorThread(&Emulator::execute, emulator);
    peripherals.events();

    timers.join();
    emulatorThread.join();
    return 0;
}
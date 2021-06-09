#include <iostream>
#include <fstream>
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
    
    int speed = 1000;
    if(argc >= 3)
        speed = atoi(argv[2]);

    ifstream romFile;
    romFile.open(argv[1], ofstream::binary);
    romFile.seekg(0, romFile.end);
    size_t romsize = romFile.tellg();
    romFile.seekg(0, romFile.beg);
    uint8_t rom[romsize];
    romFile.read((char*) rom, romsize);
    romFile.close();
    
    Memory memory(rom, romsize);
    Peripherals peripherals("CHIP-8: " + string(argv[1]));
    Emulator emulator(&peripherals, &memory, speed);

    thread timers(&Emulator::timers, emulator);
    thread emulatorThread(&Emulator::execute, emulator);
    peripherals.events();

    timers.join();
    emulatorThread.join();
    return 0;
}
#include <unistd.h>

#include "emulator.hpp"

using namespace std;

Emulator::Emulator(Display* display, Memory* memory) {
    this->display = display;
    this->memory = memory;
}

void Emulator::execute() {
    while(display->running) {
        uint16_t instruction = (memory->memory[memory->pc] << 8) | memory->memory[memory->pc+1];
        uint8_t arg1 = (instruction & 0x0F00) >> 8;
        uint8_t arg2 = (instruction & 0x00F0) >> 4;
        uint8_t arg3 = instruction & 0x000F;

        memory->pc += 2;

        switch(instruction & 0xF000) {
            case 0x0000:
                switch(instruction) {
                    case 0x00E0:
                        display->clear();
                        break;
                    case 0x00EE:
                        memory->pc = memory->stack[memory->stackPointer];
                        memory->stackPointer--;
                        break;
                }
                break;
            case  0x1000:
                memory->pc = instruction & 0x0FFF;
                break;
            case 0x2000:
                memory->stackPointer++;
                memory->stack[memory->stackPointer] = memory->pc;
                memory->pc = instruction & 0x0FFF;
                break;
            case 0x3000:
                if(memory->registers[arg1] == (instruction & 0x00FF))
                    memory->pc += 2;
                break;
            // case 0x4000:

            //     break;
            // case 0x5000:

            //     break;
            case 0x6000:
                memory->registers[arg1] = instruction & 0x00FF;
                break;
            case 0x7000:
                memory->registers[arg1] += instruction & 0x00FF;
                break;
            // case 0x8000:

            //     break;
            // case 0x9000:

            //     break;
            case 0xA000:
                memory->i = instruction & 0x0FFF;
                break;
            // case 0xB000:

            //     break;
            // case 0xC000:

            //     break;
            case 0xD000:
                int x = memory->registers[arg1 % width];
                int y = memory->registers[arg2 % height];
                int n = arg3;
                uint8_t* sprite = new uint8_t[n];
                memcpy(&sprite[0], &memory->memory[memory->i], n);
                memory->registers[0xF] = display->drawSprite(sprite, x, y, n);
                break;
            // case 0xE000:

            //     break;
            // case 0xF000:

            //     break;
        }

        usleep(10000000/speed);
    }
}
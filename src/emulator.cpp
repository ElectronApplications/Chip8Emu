#include <unistd.h>

#include "emulator.hpp"
#include "peripherals.hpp"

using namespace std;

Emulator::Emulator(Peripherals* peripherals, Memory* memory, int speed) {
    this->peripherals = peripherals;
    this->memory = memory;
    this->speed = speed;
}

void Emulator::timers() {
    while(peripherals->running) {
        if(memory->delayTimer > 0)
            memory->delayTimer--;
        if(memory->soundTimer > 0) {
            memory->soundTimer--;
            peripherals->beep();
        }
        usleep(1000000/60);
    }
}

void Emulator::execute() {
    while(peripherals->running) {
        uint16_t instruction = (memory->memory[memory->pc] << 8) | memory->memory[memory->pc+1];
        uint8_t arg1 = (instruction & 0x0F00) >> 8;
        uint8_t arg2 = (instruction & 0x00F0) >> 4;
        uint8_t arg3 = instruction & 0x000F;

        memory->pc += 2;

        switch(instruction & 0xF000) {
            case 0x0000:
                switch(instruction) {
                    case 0x00E0:
                        peripherals->clear();
                        break;
                    case 0x00EE:
                        memory->stackPointer--;
                        memory->pc = memory->stack[memory->stackPointer];
                        break;
                }
                break;
            case  0x1000:
                memory->pc = instruction & 0x0FFF;
                break;
            case 0x2000:
                memory->stack[memory->stackPointer] = memory->pc;
                memory->stackPointer++;
                memory->pc = instruction & 0x0FFF;
                break;
            case 0x3000:
                if(memory->registers[arg1] == (instruction & 0x00FF))
                    memory->pc += 2;
                break;
            case 0x4000:
                if(memory->registers[arg1] != (instruction & 0x00FF))
                    memory->pc += 2;
                break;
            case 0x5000:
                if(memory->registers[arg1] == memory->registers[arg2])
                    memory->pc += 2;
                break;
            case 0x6000:
                memory->registers[arg1] = instruction & 0x00FF;
                break;
            case 0x7000:
                memory->registers[arg1] += instruction & 0x00FF;
                break;
            case 0x8000:
                switch(arg3) {
                    case 0x0:
                        memory->registers[arg1] = memory->registers[arg2];
                        break;
                    case 0x1:
                        memory->registers[arg1] |= memory->registers[arg2];
                        break;
                    case 0x2:
                        memory->registers[arg1] &= memory->registers[arg2];
                        break;
                    case 0x3:
                        memory->registers[arg1] ^= memory->registers[arg2];
                        break;
                    case 0x4:
                        memory->registers[0xF] = ((int16_t)memory->registers[arg1] + (int16_t)memory->registers[arg2]) > 255;
                        memory->registers[arg1] += memory->registers[arg2];
                        break;
                    case 0x5:
                        memory->registers[0xF] = memory->registers[arg1] > memory->registers[arg2];
                        memory->registers[arg1] -= memory->registers[arg2];
                        break;
                    case 0x6:
                        if(SHIFT_SET_ENABLE)
                            memory->registers[arg1] = memory->registers[arg2];
                        memory->registers[0xF] = memory->registers[arg1] & 0x01;
                        memory->registers[arg1] >>= 1;
                        break;
                    case 0x7:
                        memory->registers[0xF] = memory->registers[arg2] > memory->registers[arg1];
                        memory->registers[arg2] -= memory->registers[arg1];
                        break;
                    case 0xE:
                        if(SHIFT_SET_ENABLE)
                            memory->registers[arg1] = memory->registers[arg2];
                        memory->registers[0xF] = (memory->registers[arg1] & 0b10000000) >> 7;
                        memory->registers[arg1] <<= 1;
                        break;
                }
                break;
            case 0x9000:
                if(memory->registers[arg1] != memory->registers[arg2])
                    memory->pc += 2;
                break;
            case 0xA000:
                memory->i = instruction & 0x0FFF;
                break;
            case 0xB000:
                memory->pc += instruction & 0x0FFF;
                if(OFFSET_JUMP_REG_ENABLE)
                    memory->pc += memory->registers[arg1];
                break;
            case 0xC000:
                memory->registers[arg1] = (rand() % 256) & (instruction & 0x00FF);
                break;
            case 0xD000: {
                int x = memory->registers[arg1] % WIDTH;
                int y = memory->registers[arg2] % HEIGHT;
                memory->registers[0xF] = peripherals->drawSprite(&memory->memory[memory->i], x, y, arg3);
                break;
            }
            case 0xE000:
                switch(instruction & 0x00FF) {
                    case 0x9E:
                        if(peripherals->isKeyPressed(memory->registers[arg1]))
                            memory->pc += 2;
                        break;
                    case 0xA1:
                        if(!peripherals->isKeyPressed(memory->registers[arg1]))
                            memory->pc += 2;
                        break;
                }
                break;
            case 0xF000:
                switch(instruction & 0x00FF) {
                    case 0x07:
                        memory->registers[arg1] = memory->delayTimer;
                        break;
                    case 0x15:
                        memory->delayTimer = memory->registers[arg1];
                        break;
                    case 0x18:
                        memory->soundTimer = memory->registers[arg1];
                        break;
                    case 0x1E:
                        memory->i += memory->registers[arg1];
                        if(memory->i >= 0x1000)
                            memory->registers[0xF] = 1;
                        break;
                    case 0x0A:
                        memory->registers[arg1] = peripherals->getKey();
                        break;
                    case 0x29:
                        memory->i = FONT_START + ((uint16_t)memory->registers[arg1] * 5);
                        break;
                    case 0x33:
                        memory->memory[memory->i] = memory->registers[arg1] / 100;
                        memory->memory[memory->i+1] = memory->registers[arg1] / 10 % 10;
                        memory->memory[memory->i+2] = memory->registers[arg1] % 10;
                        break;
                    case 0x55:
                        memcpy(&memory->memory[memory->i], &memory->registers[0], arg1*2+2);
                        break;
                    case 0x65:
                        memcpy(&memory->registers[0], &memory->memory[memory->i], arg1*2+2);
                        break;
                }
                break;
        }

        usleep(10000000/speed);
    }
}
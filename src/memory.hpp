#ifndef MEMORY_H
#define MEMORY_H

#include <string>

#define MEMORY_SIZE 4096
#define FONT_START 0x50
#define ROM_START 0x200
#define STACK_SIZE 32

struct Memory {
    uint8_t memory[MEMORY_SIZE];
    uint16_t stack[STACK_SIZE];
    uint8_t registers[16];
    uint16_t pc = ROM_START;
    uint16_t i = 0;
    uint8_t stackPointer = 0;
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;
    
    Memory(uint8_t* rom, size_t romsize);
};

#endif
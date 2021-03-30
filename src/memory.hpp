#ifndef MEMORY_H
#define MEMORY_H

#include <string>

class Memory {
    public:
        uint8_t* memory;
        uint8_t* registers;
        uint16_t* stack;
        uint16_t pc;
        uint16_t i;
        uint8_t stackPointer;
        
        virtual ~Memory();
        Memory(std::string path);
};

const int memorySize = 4096;
const int fontStart = 0x50;
const int romStart = 0x200;
const int stackSize = 16;

#endif

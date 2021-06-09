#ifndef EMULATOR_H
#define EMULATOR_H

#include "peripherals.hpp"
#include "memory.hpp"

#define SHIFT_SET_ENABLE true         //if enabled VX is set to VY in instructions 8XY6 and 8XYE
#define OFFSET_JUMP_REG_ENABLE false  //if enabled the program will jump to XNN plus VX in instruction BXNN, instead it will work as BNNN and jump to NNN

class Emulator {
    private:
        Peripherals* peripherals;
        Memory* memory;
        int speed = 700;  //instructions per second
    public:
        Emulator(Peripherals* peripherals, Memory* memory, int speed);
        void timers();
        void execute();
};

#endif
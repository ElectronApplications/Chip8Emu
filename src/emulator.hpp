#include "peripherals.hpp"
#include "memory.hpp"

class Emulator {
    private:
        Peripherals* peripherals;
        Memory* memory;
    public:
        Emulator(Peripherals* peripherals, Memory* memory);
        void timers();
        void execute();
};

const int speed = 700; //instructions per second
const bool ShiftSetEnable = true; //if enabled VX is set to VY in instructions 8XY6 and 8XYE
const bool OffsetJumpRegEnable = false; //if enabled the program will jump to XNN plus VX in instruction BXNN, instead it will work as BNNN and jump to NNN
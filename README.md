# Chip8Emu
Emulator  of CHIP-8 - an 8-bit architecture from 80-s that has 35 instructions (opcodes) written in C++.

# Build

# GNU/Linux
Dependencies SDL2 and SDL2_Mixer are required to compile and run the program, so you'll need to install them with your package manager.\
Then the program can be compiled with the command `make` which will use GCC compiler, to use another compiler use the command `make <CC>`, where `<CC>` is the name of the compiler you want to use (e.g. clang).\
'chip8' executable will be generated in the 'build' folder.

# Windows
To compiler the program for Windows you'll need to use MinGW-W64 compiler (port of GCC), MSVC is not supported.\
Download SDL2 and SDL2_Mixer libraries for MinGW and put '.lib' files in the 'lib' folder, '.h' files in the 'include' folder and '.dll' files in the 'build' folder.\
Then use the following command:\
`g++ -g src/*.cpp -o build/chip8.exe -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -pthread`\
'chip8.exe' executable will be generated in the 'build' folder.

# How to use
To run the program use the following command:\
`./chip8(.exe) <rom> <speed>`\
Where `<rom>` is the path to `*.ch8` rom and `<speed>` is the speed at which the emulator should work (instructions per second).
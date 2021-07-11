CXX?=g++
CFLAGS?=-O2

LIBS:=-lSDL2 -lSDL2_mixer
CFLAGS:=$(CFLAGS) -Wall -Wextra -pthread

all:
	${CXX} src/*.cpp -o build/chip8 ${LIBS} ${CFLAGS}
clean:
	rm -f build/chip8

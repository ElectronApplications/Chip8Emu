CXX?=g++
CXXFLAGS?=-O2

LIBS:=-lSDL2 -lSDL2_mixer
CXXFLAGS:=$(CXXFLAGS) -Wall -Wextra -pthread

all:
	${CXX} src/*.cpp -o build/chip8 ${LIBS} ${CXXFLAGS}
clean:
	rm -f build/chip8

CC = g++
LIBS = -lSDL2 -lSDL2_mixer

all:
	${CC} -g src/*.cpp -o build/chip8 ${LIBS} -pthread
clean:
	rm -f build/chip8
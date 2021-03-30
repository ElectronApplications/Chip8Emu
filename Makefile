CC = clang++
LIBS = -lSDL2 -lSDL2_mixer

all:
	${CC} -g src/*.cpp -o chip8 ${LIBS} -pthread
clean:
	rm -f chip8
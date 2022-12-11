CC = gcc
CPPFLAGS = -I include/ -I include/imgprocess -I include/solver -I include/neuralnetwork
CFLAGS = -Wall -Wextra -Werror -std=c99 -D__NO_INLINE__
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0` -lm

all: main

# main build

FILES = $(wildcard src/*.c) $(wildcard src/*/*.c)
SRC = $(filter-out %/main_network.c %/main_solver.c,$(FILES))
OBJ = ${SRC:.c=.o}

# main build

$(OBJ): $(notdir %.o): %.c

src/loader.o: CFLAGS += `pkg-config --cflags sdl2 SDL2_image`
src/main.o: CFLAGS += `pkg-config --cflags gtk+-3.0`

src/main: CFLAGS += -O2
src/main: ${OBJ}

main: src/main
	mv src/main .

# network build

SRC_N = $(wildcard src/neuralnetwork/*.c)
OBJ_N = $(SRC_N:.c=.o)

src/neuralnetwork/main_network: ${OBJ_N}
network: src/neuralnetwork/main_network
	mv src/neuralnetwork/main_network network

# debug build

debug: CFLAGS += -g -DDEBUG
debug: main

# test build

SRC_TEST = $(filter-out src/main.c,$(SRC)) $(wildcard tests/*.c)
OBJ_TEST = ${SRC_TEST:.c=.o}

tests/test: CFLAGS += -g -DDEBUG
tests/test: ${OBJ_TEST}
test: tests/test
	mv tests/test .

# solver build

SRC_SOLVER = $(wildcard src/solver/*.c)
OBJ_SOLVER = ${SRC_SOLVER:.c=.o}

src/solver/main_solver: ${OBJ_SOLVER}
solver: src/solver/main_solver
	mv src/solver/main_solver ./solver

# clean

clean:
	${RM} $(wildcard src/*.o) $(wildcard src/*/*.o)
	${RM} main
	${RM} network
	${RM} test
	${RM} solver

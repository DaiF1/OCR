CC = gcc
CPPFLAGS = -I include/ -I include/neuralnetwork
CFLAGS = -Wall -Wextra -Werror -std=c99 -D__NO_INLINE__
LDFLAGS =
LDLIBS = -lm

all: src/main

# main build

FILES = $(wildcard src/*.c) $(wildcard src/*/*.c)
SRC = $(filter-out %/network.c %/solver.c,$(FILES))
OBJ = ${SRC:.c=.o}

$(OBJ): $(notdir %.o): %.c

src/main: CFLAGS += -O2
src/main: ${OBJ} 

# network build

SRC_N = $(wildcard src/neuralnetwork/*.c)
OBJ_N = $(SRC_N:.c=.o)

src/neuralnetwork/network: ${OBJ_N}
network: src/neuralnetwork/network
	mv src/neuralnetwork/network .

# debug build

debug: CFLAGS += -g -DDEBUG
debug: src/main

# test build

SRC_TEST = $(filter-out src/main.c,$(wildcard src/*.c)) $(wildcard tests/*.c)
OBJ_TEST = ${SRC_TEST:.c=.o}

tests/test: ${OBJ_TEST}
test: tests/test

# clean

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_TEST}
	${RM} src/main
	${RM} network
	${RM} tests/test

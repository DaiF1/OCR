CC = gcc
CPPFLAGS = -I include/ -I include/solver
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
LDLIBS =

all: src/main

SRC = $(wildcard src/*.c) \
	$(filter-out %/main_solver.c, $(wildcard src/solver/*.c))
OBJ = ${SRC:.c=.o}

SRC_TEST = $(filter-out src/main.c,$(SRC)) $(wildcard tests/*.c)
OBJ_TEST = ${SRC_TEST:.c=.o}

SRC_SOLVER = $(wildcard src/solver/*.c)
OBJ_SOLVER = ${SRC_SOLVER:.c=.o}

# main build

$(OBJ): $(notdir %.o): %.c

src/main: ${OBJ:.c=.o}

# debug build

debug: CFLAGS += -g
debug: src/main

# test build

tests/test: ${OBJ_TEST}
test: tests/test

# solver build

src/solver/main_solver: ${OBJ_SOLVER}
solver: src/solver/main_solver
	mv src/solver/main_solver ./solver

# clean

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_TEST}
	${RM} src/main
	${RM} tests/test
	${RM} solver

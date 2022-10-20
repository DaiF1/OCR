CC = gcc
CPPFLAGS = -I include/ -I include/solver
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
LDLIBS =

all: src/main

SRC = $(wildcard src/*.c) $(wildcard src/solver/*.c)
OBJ = ${SRC:.c=.o}

SRC_TEST = $(filter-out src/main.c,$(wildcard src/*.c)) \
	$(wildcard src/solver/*.c) $(wildcard tests/*.c)
OBJ_TEST = ${SRC_TEST:.c=.o}

SRC_SOLVER = $(wildcard src/solver/*.c)
OBJ_SOLVER = ${SRC_SOLVER:.c=.o}

$(OBJ): $(notdir %.o): %.c

src/main: ${OBJ:.c=.o}

debug: CFLAGS += -g
debug: src/main

tests/test: ${OBJ_TEST}

test: tests/test

src/solver/solver: ${OBJ_SOLVER}
solver: src/solver/solver

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_TEST}
	${RM} src/main
	${RM} tests/test

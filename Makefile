CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
LDLIBS =

all: src/main

SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

SRC_TEST = $(filter-out src/main.c,$(wildcard src/*.c)) $(wildcard tests/*.c)
OBJ_TEST = ${SRC_TEST:.c=.o}

$(OBJ): $(notdir %.o): %.c

src/main: ${OBJ} 

debug: CFLAGS += -g
debug: src/main

tests/test: ${OBJ_TEST}

test: tests/test

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_TEST}
	${RM} src/main
	${RM} tests/test

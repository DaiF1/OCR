CC = gcc
CPPFLAGS = -MMD -I include/
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
LDLIBS =

all: src/main

# main build

SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

$(OBJ): $(notdir %.o): %.c

src/main: ${OBJ} 

# debug build

debug: CFLAGS += -g
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
	${RM} tests/test

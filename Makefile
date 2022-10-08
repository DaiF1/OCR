CC = gcc
CPPFLAGS = -I include/ -I include/imgprocess
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: src/main

# main build

SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

$(OBJ): $(notdir %.o): %.c

src/loader.o: CFLAGS += `pkg-config --cflags sdl2 SDL2_image`

src/main: ${OBJ} 

# debug build

debug: CFLAGS += -g
debug: src/main

# test build

SRC_TEST = $(filter-out src/main.c,$(wildcard src/*.c)) $(wildcard tests/*.c)
OBJ_TEST = ${SRC_TEST:.c=.o}

tests/test: CFLAGS += -g
tests/test: ${OBJ_TEST}
test: tests/test

# clean

clean:
	${RM} ${OBJ}
	${RM} ${OBJ_TEST}
	${RM} src/main
	${RM} tests/test

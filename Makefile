CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -MMD -MP
SDLFLAGS = $(shell pkg-config --cflags --libs sdl3)
TARGET = build/chip8
OBJ = build/main.o build/chip8.o build/display.o

.PHONY: all clean

all: $(TARGET)

build:
	mkdir -p build

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(SDLFLAGS) -o $(TARGET)

build/main.o: src/main.c src/chip8.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/chip8.o: src/chip8.c src/chip8.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/display.o: src/display.c src/chip8.h | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*

-include $(OBJ:.o=.d)

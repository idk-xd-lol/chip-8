# CHIP-8 Emulator

A CHIP-8 emulator written on C

Supports graphics, keypad input, timers, sound and most standard CHIP-8 ROMs.


## Features

- CHIP-8 CPU implementation
- 64x32 display
- SDL3 graphics
- Keyboard input
- Delay timer
- Sound timer
- ROM loading

## Controls

CHIP-8 keypad:

1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F

Keyboard mapping:

1 2 3 4
Q W E R
A S D F
Z X C V

## Build

Requirements:

- GCC
- SDL3

Build:

```bash
make
```
---

# Run

```bash
./build/chip8 roms/games/Tetris [Fran Dachille, 1991].ch8
```

---

# Technical details

This section looks professional.

```md
## Technical details

- Language: C
- Graphics: SDL3
- Display: 64x32 monochrome
- Memory: 4096 bytes
- Stack: 16 levels
```

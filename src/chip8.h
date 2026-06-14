#ifndef CHIP8_H
#define CHIP8_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

typedef struct {
  uint8_t memory[0x1000];
  
  uint8_t V[0x10];

  uint16_t I;
  uint16_t pc;

  uint16_t stack[0x10];
  uint8_t sp;

  uint8_t delay_timer;
  uint8_t sound_timer;

} Chip8;

uint16_t read_opcode(Chip8 *chip);
void exec_opcode(Chip8 *chip, int opcode);
void fileopen(char *filename, Chip8 *chip);
void unknown_opcode(int opcode);
void execute_cpu_cycle(Chip8 *chip);
void window_create();
void window_destroy();

#endif // !i

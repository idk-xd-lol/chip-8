#ifndef CHIP8_H
#define CHIP8_H

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>

typedef struct {
  uint8_t memory[0x1000];
  
  uint8_t V[0x10];

  uint16_t I;
  uint16_t pc;

  uint16_t stack[0x10];
  uint8_t sp;

  uint8_t delay_timer;
  uint8_t sound_timer;

  bool display[32][64];

  bool waiting_for_key;
  uint8_t waiting_register;

} Chip8;

static const unsigned char font[0x50] = 
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void fileopen(char *filename, Chip8 *chip);

void exec_opcode(Chip8 *chip, int opcode);
void unknown_opcode(int opcode);
uint16_t read_opcode(Chip8 *chip);

void execute_cpu_cycle(Chip8 *chip);
void event_loop(Chip8 *chip, SDL_Event *event, bool *running);
void render_draw(SDL_Renderer *renderer);
void set_grid(Chip8 *chip, uint8_t x, uint8_t y, uint8_t n);

uint8_t keymap(SDL_Keycode key);
extern bool keys[];

#endif // !i

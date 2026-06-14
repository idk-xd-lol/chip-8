#ifndef CHIP8_H
#define CHIP8_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

#endif // !i

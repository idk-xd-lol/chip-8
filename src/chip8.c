#include "chip8.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>


uint16_t read_opcode(Chip8 * chip)
{
  return (chip->memory[chip->pc] << 8) | chip->memory[chip->pc+1];
}

void exec_opcode(Chip8* chip, int opcode)
{
  uint8_t x = (opcode >> 8) & 0x000F;
  uint8_t y = (opcode >> 4) & 0x000F;
  uint8_t n = opcode & 0x000F; 
  uint8_t nn = opcode & 0x00FF;
  uint16_t nnn = opcode & 0x0FFF;
  uint16_t result;
  switch (opcode & 0xF000)
  {
    case 0x0000:
      switch(nn)
      {
        case 0xE0:
          //clears screen
          break;
        case 0xEE:
          chip->pc = chip->stack[--chip->sp];
          break;
        default:
          unknown_opcode(opcode);
          break;
      }
      break;
    case 0x1000:
      chip->pc = nnn; //jump to nnn
      break;
    case 0x2000:
      chip->stack[chip->sp++] = chip->pc;
      chip->pc = nnn;
      break;
    case 0x3000:
      if(chip->V[x] == nn)
        chip->pc += 2;
      break;
    case 0x4000:
      if(chip->V[x] != nn)
        chip->pc += 2;
      break;
    case 0x5000:
      if(chip->V[x] == chip->V[y])
        chip->pc +=2;
      break;
    case 0x6000:
      chip->V[x] = nn;
      break;
    case 0x7000:
      chip->V[x] += nn;
      break;
    case 0x8000:
      switch(n)
      {
        case 0x0:
          chip->V[x] = chip->V[y];
          break;
        case 0x1:
          chip->V[x] |= chip->V[y];
          break;
        case 0x2:
          chip->V[x] &= chip->V[y];
          break;
        case 0x3:
          chip->V[x] ^= chip->V[y];
          break;
        case 0x4:
          result = chip->V[x] + chip->V[y];
          chip->V[0xF] = result > 0xFF;
          chip->V[x] = result & 0xFF;
          break;
        case 0x5:
          chip->V[0xF] = chip->V[x] >= chip->V[y];
          chip->V[x] -= chip->V[y];
          break;
        case 0x6:
          chip->V[0xF] = chip->V[x] & 1;
          chip->V[x] >>= 1;
          break;
        case 0x7:
          chip->V[0xF] = chip->V[y] >= chip->V[x];
          chip->V[x] =  chip->V[y] - chip->V[x];
          break;
        case 0xE:
          chip->V[0xF] = (chip->V[x] & 0x80) >> 7;
          chip->V[x] <<= 1;
          break;
        default:
          unknown_opcode(opcode);
          break;
      }
      break;
    case 0x9000:
      if(chip->V[x] != chip->V[y])
        chip->pc +=2;
      break;
    case 0xA000:
      chip->I = nnn;
      break;
    case 0xB000:
      chip->pc = chip->V[0x0] + nnn;
      break;
    case 0xC000:
      chip->V[x] = nnn & (rand() % 0xFF);
      break;
    case 0xD000:
      //draw sprite
      break;
    case 0xE000:
      switch (nn)
      {
        case 0x9E:
          //pc+=2 if key in VX is pressed
          break;
        case 0xA1:
          //pc+=2 if key in VX isn't pressed
          break;
        default:
          unknown_opcode(opcode);
          break;
      }
      break;
    case 0xF000:
      switch (nn)
      {
        case 0x07:
          chip->V[x] = chip->delay_timer;
          break;
        case 0x0A:
          //a key press is awaited and then stored in VX
          break;
        case 0x15:
          //sets the delay timer to VX
          chip->delay_timer = chip->V[x];
          break;
        case 0x18:
          //sets the sound timer to VX 
          chip->sound_timer = chip->V[x];
          break;
        case 0x1E:
          chip-> I += chip->V[x];
          break;
        case 0x29:
          //sets I to the location of the sprite for the character in VX
          break;
        case 0x33:
          //stores the binary coded decimal represintation of VX
          break;
        case 0x55:
          //stores from V0 to VX(including VX) in memory starting at address I. 
          break;
        case 0x65:
          //fills from V0 to VX with values from memory starting at address I.
          break;
        default:
          unknown_opcode(opcode);
          break;
        break;
      }
    default:
      unknown_opcode(opcode);
      break;
  }
  printf("0x%X\n", opcode);
}

void fileopen(char *filename, Chip8 *chip)
{
  int c;
  FILE *fp = fopen(filename, "rb"); //open file
  if(!fp)
  {
    perror("No such file");
    exit(EXIT_FAILURE);
  }

  //check file extention
  char *ext = strrchr(filename, '.');
  if(!ext || strcmp(ext, ".ch8"))
  {
    printf("Error: Wrong file format\n");
    exit(EXIT_FAILURE);
  }

  //load into memory
  for (int i = chip->pc; i < 0x1000 && (c = fgetc(fp)) != EOF; i++)
  {
    chip->memory[i] = c;
  }
  fclose(fp);
}

void execute_cpu_cycle(Chip8 *chip)
{
  uint16_t opcode = read_opcode(chip);
  chip->pc += 2;
  exec_opcode(chip, opcode);
}

void unknown_opcode(int opcode)
{
  printf("error, unknown opcode: 0x%x", opcode);
  exit(EXIT_FAILURE);
}

void event_loop(SDL_Event *event, bool *running)
{
  while (SDL_PollEvent(event)) {
      if(event->type == SDL_EVENT_QUIT)
        running = false;
    }
}

void render_draw(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

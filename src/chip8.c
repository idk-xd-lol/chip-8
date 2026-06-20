#include "chip8.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>
#include <stdlib.h>

bool keys[16] = {0};

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
      chip->pc = nnn;
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
      set_grid(chip, x, y, n);
      break;
    case 0xE000:
      switch (nn)
      {
        case 0x9E:
          if(keys[x])
            chip->pc += 2;
          break;
        case 0xA1:
          if(!keys[x])
            chip->pc+=2;
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
          chip->waiting_for_key = true;
          chip->waiting_register = x;
          break;
        case 0x15:
          chip->delay_timer = chip->V[x];
          break;
        case 0x18:
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
      }
      break;
    default:
      unknown_opcode(opcode);
      break;
  }
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
  printf("error, unknown opcode: 0x%X", opcode);
  exit(EXIT_FAILURE);
}

void event_loop(Chip8 * chip, SDL_Event *event, bool *running)
{
  while (SDL_PollEvent(event)) {
      if(event->type == SDL_EVENT_QUIT)
        running = false;
      if(event->type == SDL_EVENT_KEY_DOWN)
      {
        uint8_t k = keymap(event->key.key);
        if (k != 0xFF)
        {
           keys[k] = true;
           if(chip->waiting_for_key)
           {
             chip->V[chip->waiting_register] = k;
             chip->waiting_for_key = false;
           }
        }
      }
      if(event->type == SDL_EVENT_KEY_UP)
      {
        uint8_t k = keymap(event->key.key);
        if (k != 0xFF)
          keys[k] = false;
      }
    }
}

void render_draw(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

uint8_t keymap(SDL_Keycode key)
{
  switch (key)
  {
    case SDLK_1: return 0x1;
    case SDLK_2: return 0x2;
    case SDLK_3: return 0x3;
    case SDLK_4: return 0xC;

    case SDLK_Q: return 0x4;
    case SDLK_W: return 0x5;
    case SDLK_E: return 0x6;
    case SDLK_R: return 0xD;

    case SDLK_A: return 0x7;
    case SDLK_S: return 0x8;
    case SDLK_D: return 0x9;
    case SDLK_F: return 0xE;

    case SDLK_Z: return 0xA;
    case SDLK_X: return 0x0;
    case SDLK_C: return 0xB;
    case SDLK_V: return 0xF;

    default: return 0xFF;
  }
}


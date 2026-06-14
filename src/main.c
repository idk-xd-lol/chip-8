#include "chip8.h"
#include <SDL3/SDL_events.h>

unsigned char font[0x50] = 
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


int main(int argc, char **argv)
{
  //check argument count
  if (argc != 2)
  {
    printf("USAGE: ./chip8 <path_to_file>\n");
    exit(EXIT_FAILURE);
  }

  Chip8 chip = {0};
  chip.pc = 0x200;
  bool running = true;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("CHIP-8", 640, 320, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  SDL_Event event;

  fileopen(argv[1], &chip); //open file
  memcpy(&chip.memory[0x50], font, sizeof(font)); //load font
  srand(time(NULL));

  //screen 
  Uint16 last_timer = SDL_GetTicks();

  while(running)
  {
    while (SDL_PollEvent(&event)) {
      if(event.type == SDL_EVENT_QUIT)
        running = false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    execute_cpu_cycle(&chip);
    Uint16 now = SDL_GetTicks();
    if(now - last_timer >= 1000 / 60)
    {
      if(chip.delay_timer > 0)
        chip.delay_timer--;
      if (chip.sound_timer > 0)
        chip.sound_timer--;

      last_timer = now;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

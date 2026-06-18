#include "chip8.h"

int main(int argc, char **argv)
{
  //check argument count
  if (argc != 2)
  {
    printf("USAGE: ./chip8 <path_to_file>\n");
    exit(EXIT_FAILURE);
  }

  bool keys[16] = {0};
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
    event_loop(&event, &running);
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

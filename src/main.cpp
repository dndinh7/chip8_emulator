/**
 * This serves as the main entry point of the chip8 emulator
*/
#include <iostream>
#include "chip8.h"
#include <SDL.h>
#include <GL/glut.h>

Chip8 chip8;

int main() {
  const int WIDTH= 64;
  const int HEIGHT= 32;
  const int SCALE= 20;

  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "Error initializing SDL" << std::endl;
    return 1;
  }
  
  SDL_Window* win= SDL_CreateWindow("Game Canvas",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WIDTH * SCALE, HEIGHT * SCALE, 
                                     SDL_WINDOW_SHOWN);

  if (win == nullptr) {
    std::cout << "Could not initialize window" << std::endl;
    return 1;
  }

  SDL_Delay(3000);

  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;

}
#ifdef WIN32
  #define SDL_MAIN_HANDLED
#endif

#include <iostream>
#include <cstdlib>
#include "Arena.h"
#include <SDL.h>


int main()
{

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "error init sdl\n";
    return EXIT_FAILURE;
  }

  auto window = SDL_CreateWindow("Game",0, 0, 20, 20, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);



  //  Arena arena(20, 20);
  Arena arena("maps/map1.png");
  SDL_Event event;
  bool quit = false;
  int xDir = 0;
  int yDir = 0;

  while (!quit)
  {

    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_QUIT: quit = true; break;
      case SDL_KEYDOWN :
        switch (event.key.keysym.sym)
        {
          case SDLK_ESCAPE: quit = true; break;
          case SDLK_LEFT:  xDir = -1; break;
          case SDLK_RIGHT: xDir = 1; break;
          case SDLK_UP:    yDir = -1; break;
          case SDLK_DOWN:  yDir =  1; break;
        }
      break; // end key down

      case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
          case SDLK_LEFT: xDir = 0; break;
          case SDLK_RIGHT: xDir = 0; break;
          case SDLK_UP: yDir = 0; break;
          case SDLK_DOWN: yDir = 0; break;

        }
        break; // end key down

      }
    } // end poll event

    arena.keyEvent(xDir, yDir);
    arena.update(0.1f);
    arena.draw();
    SDL_Delay(10);
  } // end game loop


  return EXIT_SUCCESS;
}
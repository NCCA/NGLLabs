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
  arena.setItem(4, 4, ItemType::Player);
  SDL_Event event;
  bool quit = false;
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
      
        }
      break; // end key down
      }
    } // end poll event


    arena.update(0.1f);
    arena.draw();
  } // end game loop


  return EXIT_SUCCESS;
}
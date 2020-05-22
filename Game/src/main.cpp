#ifdef WIN32
  #define SDL_MAIN_HANDLED
#endif

#include <iostream>
#include <cstdlib>
#include "Arena.h"
#include <SDL.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "RenderGlobals.h"
#include <ngl/NGLStream.h>

SDL_GLContext createOpenGLContext(SDL_Window* _window);
void setupShaders();

static int WIDTH = 1024;
static int HEIGHT = 720;
static float ASPECT = static_cast<float>(WIDTH) / HEIGHT;
int main()
{

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "error init sdl\n";
    return EXIT_FAILURE;
  }

  auto window = SDL_CreateWindow("Game",0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL );

  auto glContext = createOpenGLContext(window);
  if (!glContext)
  {
    std::cerr << "can't create GL context \n";
    exit(EXIT_FAILURE);
  }
  SDL_GL_MakeCurrent(window, glContext);
  ngl::NGLInit::instance();
  setupShaders();
  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  RenderGlobals::setView({ 0.0f,10.0f,-20.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f });
  RenderGlobals::setProjection(45.0f, ASPECT);

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
          case SDLK_LEFT:  xDir = 1; break;
          case SDLK_RIGHT: xDir = -1; break;
          case SDLK_UP:    yDir = 1; break;
          case SDLK_DOWN:  yDir =  -1; break;
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WIDTH, HEIGHT);
    arena.keyEvent(xDir, yDir);
    arena.update(0.1f);
    arena.draw();
    //auto shader = ngl::ShaderLib::instance();
    //shader->setUniform("MV", RenderGlobals::getViewMatrix());
    //shader->setUniform("MVP", RenderGlobals::getVPMatrix());

    //ngl::VAOPrimitives::instance()->draw("teapot");
    SDL_GL_SwapWindow(window);
    SDL_Delay(10);
  } // end game loop


  return EXIT_SUCCESS;
}


SDL_GLContext createOpenGLContext(SDL_Window* _window)
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  return SDL_GL_CreateContext(_window);

}


void setupShaders()
{
  auto shader = ngl::ShaderLib::instance();
  shader->use(ngl::nglDiffuseShader);
  shader->setUniform("Colour", 1.0f, 0.0f, 0.0f, 1.0f);
  shader->setUniform("lightPos", 0.0f, 10.0f, 0.0f);
  shader->setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
  shader->setUniform("normalMatrix", ngl::Mat3());

  shader->use(ngl::nglCheckerShader);
  shader->setUniform("lightPos", 0.0f, 10.0f, 0.0f);
  shader->setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
  shader->setUniform("checkOn", true);
  shader->setUniform("colour1", 0.9f, 0.9f, 0.9f, 1.0f);
  shader->setUniform("colour2", 0.6f, 0.6f, 0.6f, 1.0f);
  shader->setUniform("checkSize", 10.0f);
  shader->setUniform("normalMatrix", ngl::Mat3());

}

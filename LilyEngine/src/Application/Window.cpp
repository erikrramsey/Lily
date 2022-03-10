#include "Application/Window.h"
namespace Lily {


  Window::Window() {
    gWindow = nullptr;
  }

  Window::~Window()
  {
    SDL_StopTextInput();
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
  }

  bool Window::Initialize(const std::string name, int width, int height)
  {
      // Start SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
      printf("SDL failed to initialize: %s\n", SDL_GetError());
      return false;
    }

    // Start OpenGL for SDL
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    // Create window
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    
    //use for fullscreen
    if (height == 0 && width == 0)
    {
      height = current.h;
      width = current.w;
    }

    gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    if(gWindow == NULL)
    {
      printf("Widow failed to create: %s\n", SDL_GetError());
      return false;
    }

    // Create context
    gContext = SDL_GL_CreateContext(gWindow);
    if(gContext == NULL)
    {
      printf("OpenGL context not created: %s\n", SDL_GetError());
      return false;
    }

    // Use VSync
    if(SDL_GL_SetSwapInterval(1) < 0) {
      printf("Unable to use VSync: %s\n", SDL_GetError());
      return false;
    }

    SDL_SetWindowResizable(gWindow, SDL_TRUE);

    return true;
  }

  void Window::Swap() {
    SDL_GL_SwapWindow(gWindow);
  }

  void Window::OnEvent(SDL_Event& ev) {
      switch (ev.type) {
      case SDL_KEYDOWN:
          switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE:
              ToggleMouseLock();
              break;
          }
      }
   }
   

  void Window::ToggleMouseLock() {
    bool enabled = !SDL_GetRelativeMouseMode();
    SDL_SetRelativeMouseMode((SDL_bool)enabled);
  }

}
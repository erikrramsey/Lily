#pragma once

#include <iostream>
#include <string>

#include <SDL.h>

#include "EventSender.h"


namespace Lily {

  class Window : public EventListener
  {
    public:
      Window();
      ~Window();
    
      void OnEvent(SDL_Event& ev) override;

      bool Initialize(const std::string name, int width, int height);

      void Swap();
      void toggle_mouselock();
      void set_vsync(int value);

      SDL_Window* gWindow;
      SDL_GLContext gContext;

    private:
  };

}

#pragma once

#include <iostream>
#include <string>

#include <SDL.h>
#include <filesystem>
#include "Renderer/stb_image.h"

#include "LilyEventSender.h"
namespace fs = std::filesystem;


namespace Lily {

  class Window : public EventListener
  {
    public:
      Window();
      ~Window();
    
      void OnEvent(SDL_Event& ev) override;

      bool initialize(const std::string& name, int width, int height);

      void swap();
      void toggle_mouselock();
      void set_vsync(int value);

      SDL_Window* gWindow;
      SDL_GLContext gContext;

    private:
  };

}

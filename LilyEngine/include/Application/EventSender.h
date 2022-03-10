#pragma once

#include <vector>

#include <SDL.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Lily {

	class EventListener {
	public:
		virtual void OnEvent(SDL_Event& ev) = 0;
	};


	class EventSender {
	  public:
		EventSender();
		~EventSender();

		void Send(SDL_Event& ev);

		void AddCallBack(EventListener* listener);

		bool initialize();
		void pollEvents();
		void parse_keydown();
		void parse_keyup();
		void parse_mouse();
		void parse_window_event();
		void toggleMouseInput();

	  private:

		std::vector<EventListener *> m_callbacks;
		SDL_Event m_sdl_event;
		bool mouse_locked;
	};
}

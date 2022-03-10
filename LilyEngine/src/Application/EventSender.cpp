#include <iostream>

#include "Application/EventSender.h"

namespace Lily {


	EventSender::EventSender() {

	}

	EventSender::~EventSender() {

	}

	bool EventSender::initialize(){
		mouse_locked = true;
		return SDL_InitSubSystem(SDL_INIT_EVENTS);
	}

	void EventSender::pollEvents(){
		while(SDL_PollEvent(&m_sdl_event) != 0) {
			Send(m_sdl_event);
		}
	}

	void EventSender::toggleMouseInput() {
		mouse_locked = !mouse_locked;
	}

	void EventSender::AddCallBack(EventListener* listener) {
		m_callbacks.push_back(listener);
	}

	void EventSender::Send(SDL_Event& ev) {
		for (auto i : m_callbacks) {
			i->OnEvent(ev);
		}
	}
}
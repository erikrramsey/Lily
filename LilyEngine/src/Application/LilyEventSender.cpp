#include <iostream>

#include "Application/LilyEventSender.h"

namespace Lily {


	LilyEventSender::LilyEventSender() {

	}

	LilyEventSender::~LilyEventSender() {

	}

	bool LilyEventSender::initialize(){
		mouse_locked = true;
		return SDL_InitSubSystem(SDL_INIT_EVENTS);
	}

	void LilyEventSender::pollEvents(){
		while(SDL_PollEvent(&m_sdl_event) != 0) {
			Send(m_sdl_event);
		}
	}

	void LilyEventSender::toggleMouseInput() {
		mouse_locked = !mouse_locked;
	}

	void LilyEventSender::AddCallBack(EventListener* listener) {
		m_callbacks.push_back(listener);
	}

	void LilyEventSender::Send(SDL_Event& ev) {
		for (auto i : m_callbacks) {
			i->OnEvent(ev);
		}
	}
}
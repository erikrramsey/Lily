#pragma once

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Application/Application.h"
#include "Application/LilyEventSender.h"


namespace Lily {
	class Gui : public EventListener{
	public:
		Gui();
		virtual ~Gui();

		virtual void init();
		virtual void update(long long dt) = 0;

		void OnEvent(SDL_Event& ev) override;

	protected:
		void Cleanup();
	};
}
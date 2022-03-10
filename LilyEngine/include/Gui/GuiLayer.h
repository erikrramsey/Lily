#pragma once

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Application/Application.h"
#include "Application/EventSender.h"


namespace Lily {
	class Application;
	class GuiLayer : public EventListener{
	public:
		GuiLayer();
		virtual ~GuiLayer();

		virtual void Init();
		virtual void Update(long long dt) = 0;

		void OnEvent(SDL_Event& ev) override;

	protected:
		void Cleanup();
		Application* m_app;
	};
}
#pragma once

#include <vector>

#include "Application/Clock.h"
#include "Application/EventSender.h"
#include "Application/Window.h"
#include "Gui/GuiLayer.h"
#include "Renderer/Renderer.h"
#include "Entity/Scene.h"

int main(int argc, char* argv);


namespace Lily {

	class GuiLayer;
	class Application : public EventListener {
	public:
		Application();
		~Application();

		void Init(const char* name = "Lily Application");
		void OnEvent(SDL_Event& ev) override;

		static Application* Get() { return s_instance; }
		Window& GetWindow() { return *m_window; }
		EventSender& GetEventSender() { return *m_event_sender; }

		void AddLayer(GuiLayer* layer);

	private:
		bool m_running;
		Window* m_window;
		EventSender* m_event_sender;
		Clock* m_clock;

		std::vector<GuiLayer*> m_layers;

		void Run();

		friend int ::main(int argc, char* argv[]);
		static Application* s_instance;
	};

	Application* CreateApplication();

}
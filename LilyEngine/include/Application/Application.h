#pragma once

#include <vector>

#include "Application/Clock.h"
#include "Application/LilyEventSender.h"
#include "Application/Window.h"
#include "Gui/Gui.h"
#include "Renderer/Renderer.h"
#include "Entity/Scene.h"

int main(int argc, char* argv);


namespace Lily {

	class Gui;
	class Application : public EventListener {
	public:
		Application();
		~Application();

		void Init(const char* name = "Lily Application");
		void OnEvent(SDL_Event& ev) override;

		static Application* get() { return s_instance; }
		Window& get_window() { return *m_window; }
		LilyEventSender& EventSender() { return *m_event_sender; }

		void AddLayer(Gui* layer);

    protected:
        int m_window_size[2] = {0, 0};

	private:
		bool m_running;
		Window* m_window;
		LilyEventSender* m_event_sender;
		Clock* m_clock;

		std::vector<Gui*> m_layers;

		void Run();

		friend int ::main(int argc, char* argv[]);
		static Application* s_instance;
	};

	Application* CreateApplication();

}
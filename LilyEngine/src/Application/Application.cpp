#include "Application/Application.h"

namespace Lily {

    Application::Application() {
        s_instance = this;
        m_window = new Window();
        m_clock = new Clock();
        m_event_sender = new EventSender();
        m_running = false;
    }

    void Application::Init(const char* name) {
        m_window->Initialize(name, 0, 0);
        m_event_sender->initialize();
        m_event_sender->AddCallBack(this);
        m_event_sender->AddCallBack(m_window);

        Renderer::Initialize("../../LilyEngine/shaders/default.vs",
            "../../LilyEngine/shaders/default.fs");

        for (auto i : m_layers) {
            i->Init();
            m_event_sender->AddCallBack(i);
        }

        m_running = true;
    }

    Application::~Application() {

    }
    
    void Application::Run() {
        while (m_running) {
            m_event_sender->pollEvents();
            for (auto layer : m_layers) {
                layer->Update(m_clock->getDT());
            }
            m_window->Swap();
        }
    }

    void Application::OnEvent(SDL_Event& ev) {
        switch (ev.type) {
        case (SDL_QUIT): 
            m_running = false;
            break;
        }
    }
    
    void Application::AddLayer(GuiLayer* Layer) {
        m_layers.push_back(Layer);
    }

}



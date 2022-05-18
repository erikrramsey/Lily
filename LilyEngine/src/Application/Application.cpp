#include "Application/Application.h"

namespace Lily {

    Application::Application() {
        s_instance = this;
        m_window = new Window();
        m_clock = new Clock();
        m_event_sender = new LilyEventSender();
        m_running = false;
    }

    void Application::Init(const char* name) {
        m_window->initialize(name, m_window_size[0], m_window_size[1]);
        m_event_sender->initialize();
        m_event_sender->AddCallBack(this);
        m_event_sender->AddCallBack(m_window);

        Renderer::Initialize();

        for (auto i : m_layers) {
            i->init();
            m_event_sender->AddCallBack(i);
        }

        m_running = true;
    }

    Application::~Application() {
        delete m_window;
        delete m_clock;
        delete m_event_sender;
    }
    
    void Application::Run() {
        while (m_running) {
            m_event_sender->pollEvents();
            for (auto layer : m_layers) {
                layer->update(m_clock->getDT());
            }
            m_window->swap();
        }
    }

    void Application::OnEvent(SDL_Event& ev) {
        switch (ev.type) {
        case (SDL_QUIT): 
            m_running = false;
            break;
        }
    }
    
    void Application::AddLayer(Gui* Layer) {
        m_layers.push_back(Layer);
    }

}



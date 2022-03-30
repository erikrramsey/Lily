#include "Gui/GuiLayer.h"

namespace Lily {

    GuiLayer::GuiLayer() {
        m_app = nullptr;
    }

    GuiLayer::~GuiLayer() {
        Cleanup();
    }
    void GuiLayer::Init() {
        const char* glsl_version = "#version 130";
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.WantCaptureKeyboard = true;
        io.IniFilename = "assets/imgui.ini";

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();


        m_app = Application::Get();
        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(m_app->GetWindow().gWindow , m_app->GetWindow().gContext);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void GuiLayer::OnEvent(SDL_Event& ev) {
        ImGui_ImplSDL2_ProcessEvent(&ev);
    }

    void GuiLayer::Cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
    }
}

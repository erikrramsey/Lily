#include "Gui/Gui.h"

namespace Lily {

    Gui::Gui() {}

    Gui::~Gui() {
        Cleanup();
    }

    void Gui::init() {
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

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(Application::get()->get_window().gWindow , Application::get()->get_window().gContext);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void Gui::OnEvent(SDL_Event& ev) {
        ImGui_ImplSDL2_ProcessEvent(&ev);
    }

    void Gui::Cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
    }
}

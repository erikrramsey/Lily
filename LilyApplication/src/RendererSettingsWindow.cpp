//
// Created by Erik on 6/27/2022.
//
#include "RendererSettingsWindow.h"

RendererSettingsWindow::RendererSettingsWindow(LilyEditor* parent) : EditorWindow(parent) {
        m_shader_directory = m_parent->get_data().selected_project.string() + "/shaders/";
}

void RendererSettingsWindow::render() {
    ImGui::Begin("Renderer Settings");

    ImGui::Text("Shaders: ");
    ImGui::BeginChild("Shaders", ImVec2(500, 500), true);
    shader_list();
    ImGui::EndChild();
    ImGui::End();
}
void RendererSettingsWindow::shader_list() {
    if (!fs::exists(m_shader_directory)) {
        ImGui::Text("No shader directory found");
        return;
    }

    static fs::path vs_path;
    for (auto entry : fs::directory_iterator(m_shader_directory)) {
        if (ImGui::Selectable(entry.path().string().c_str(), entry.path() == vs_path)) {
            vs_path = entry.path();
        }
    }

    ImGui::Dummy(ImVec2(0, 10));
    static fs::path fs_path;
    for (auto entry : fs::directory_iterator(m_shader_directory)) {
        if (ImGui::Selectable((entry.path().string() + "##").c_str(), entry.path() == fs_path)) {
            fs_path = entry.path();
        }
    }

    if (ImGui::Button("Build shaders")) {
        Renderer::build_and_use_shader(vs_path, fs_path);
    }
}

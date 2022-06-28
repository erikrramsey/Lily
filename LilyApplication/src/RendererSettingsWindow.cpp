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
    static int selection = -1;
    if (!fs::exists(m_shader_directory)) {
        ImGui::Text("No shader directory found");
        return;
    }
    auto it = 0;
    for (auto entry : fs::directory_iterator(m_shader_directory)) {
        if (ImGui::Selectable(entry.path().string().c_str(), it == selection)) {
            selection = it;
        }

        if (ImGui::IsMouseDoubleClicked(0)) {
            Renderer::build_shader(entry.path());
        }
        it++;
    }
}

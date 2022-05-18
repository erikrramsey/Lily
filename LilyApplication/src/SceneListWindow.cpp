#include "SceneListWindow.h"

SceneListWindow::SceneListWindow(LilyEditor* parent) : EditorWindow(parent) {
    m_scenes_directory = m_parent->project_path().string() + "\\Scenes";
}


void SceneListWindow::render() {
    ImGui::Begin("Scenes");
    if (ImGui::Button("New Scene")) {
        int sc = 1;
        std::string name;
        fs::path new_proj;
        do {
            name = "\\New Scene " + std::to_string(sc) + ".json";
            sc++;
            new_proj = m_parent->project_path().string() + "\\Scenes" + name;
        } while (fs::exists(new_proj));
        SceneSerializer::serialize(m_parent->active_scene(), new_proj.string());
    }

    ImGui::SameLine();
    if (ImGui::Button("Save Scene")) {

    }

    ImGui::SameLine();
    if (ImGui::Button("Load Scene")) {

    }

    int it = 0;
    for (auto& entry : fs::directory_iterator(m_scenes_directory)) {
        if (ImGui::Selectable(entry.path().filename().string().c_str(), m_selection_it == it)) {
            m_selected = entry.path();
            m_selection_it = it;
        }
        it++;
    }
    ImGui::End();
}

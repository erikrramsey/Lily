#include "SceneListWindow.h"

SceneListWindow::SceneListWindow(LilyEditor* parent) : EditorWindow(parent) {
    m_scenes_directory = m_parent->project_path().string() + "\\Scenes";
    m_renaming = false;
    m_selection_it = -1;
}

SceneListWindow::~SceneListWindow() {
    m_scenes_directory.clear();
    m_renaming = false;
    m_selection_it = -1;
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
        SceneSerializer::serialize(nullptr, new_proj.string());
    }

    ImGui::SameLine();
    if (ImGui::Button("Save Scene")) {
        SceneSerializer::serialize(m_parent->active_scene(), m_selected);
    }

    ImGui::SameLine();
    if (ImGui::Button("Load Scene")) {
        SceneSerializer::deserialize(m_parent->active_scene(), m_selected);
        m_parent->on_deserialize();
    }

    int it = 0;
    for (auto& entry : fs::directory_iterator(m_scenes_directory)) {
        if (m_renaming && m_selection_it == it) {
            char input[80] = "";
            strcpy_s(input, 80, m_selected.filename().string().c_str());
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##", input, 80, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                fs::path newPath = entry.path().parent_path().string() + "\\" + std::string(input);
                if (!newPath.has_extension()) newPath = newPath.string() + ".json";
                fs::rename(entry, newPath);
                m_selected = newPath;
                m_renaming = false;
            }
            else if (ImGui::IsItemDeactivated()) {
                m_renaming = false;
            }
        } else if (ImGui::Selectable(entry.path().filename().string().c_str(), m_selection_it == it, ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selected = entry.path();
            m_selection_it = it;
            if (ImGui::IsMouseDoubleClicked(0))
                m_renaming = true;
        }

        it++;
    }
    ImGui::End();
}

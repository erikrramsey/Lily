#include "ComponentEditorWindow.h"

const char* COMPONENTS[] = {
        "Mesh",
        "Camera"
};

ComponentEditorWindow::ComponentEditorWindow(LilyEditorLayer* parent) {
    m_parent = parent;
    m_selected = nullptr;
    m_rendered = nullptr;
    m_show_file_explorer = false;

    m_file_explorer = new LilyFileExplorer();
}

void ComponentEditorWindow::render_Lobject(Lobject* obj) {
    m_rendered = obj;
    render_Lobject();
}

void ComponentEditorWindow::render_Lobject() {
    if (m_rendered == nullptr) return;
    render();
}

void ComponentEditorWindow::render() {
    if (m_show_file_explorer) {
        m_file_explorer->render();
        auto p = m_file_explorer->get_selection();
        if (!p.empty()) {
            m_parent->m_active_scene->import_component(m_selected, p.string());
            m_show_file_explorer = false;
        }
    }
    if (m_rendered == nullptr || m_rendered->try_get<Camera>()) return;

    ImGui::Begin("Component Editor");

    ImGui::Text("%d", static_cast<int>(m_rendered->m_entity));
    ImGui::SameLine();
    const char* current = "Add Component";
    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton;
    if (ImGui::BeginCombo("##Add Component", current, flags)) {
        if (ImGui::Selectable(COMPONENTS[0])) {
            m_selected = m_rendered;
            m_show_file_explorer = true;
        }
        if (ImGui::Selectable(COMPONENTS[1])) {

        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    render_family();
    ImGui::Separator();
    render_transform();
    ImGui::Separator();
    render_mesh();

    ImGui::End();
}

void ComponentEditorWindow::render_family() {
    auto& comp = m_rendered->get<Family>();
    ImGui::Text("Family Component");
    ImGui::Dummy(ImVec2(0, 10));

    ImGui::Text("Parent");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(comp.parent));

    if (ImGui::Button("Set Root as Parent")) {
        m_parent->m_active_scene->get_root()->add_child(m_rendered);
    }

    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton;
    const char* current = "Add Child";
    if (ImGui::BeginCombo("##Add Child", current, flags)) {
        for (auto& [ent, obj] : m_parent->m_active_scene->m_objects) {
            bool is_valid = true;
            auto climb = m_rendered;
            while (climb != m_parent->m_active_scene->get_root()) {
                if (obj == climb) is_valid = false;
                climb = climb->get_parent();
            }
            for (auto ch : m_rendered->get_children()) {
                if (obj == ch) is_valid = false;
            }
            if (is_valid && ImGui::Selectable(obj->get_name())) {
                m_rendered->add_child(obj);
            }
        }
       ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0, 10));

}

void ComponentEditorWindow::render_transform() {
    auto& comp = m_rendered->get<Transform>();
    ImGui::Text("Transform Component");
    ImGui::Dummy(ImVec2(0, 10));
    glm::vec3 pos, rot, sca;
    comp.decompose(pos, rot, sca);

    float position[] = { pos.x, pos.y, pos.z };
    float rotation[] = { rot.x, rot.y, rot.z };
    float scale[]    = { sca.x, sca.y, sca.z };

    ImGui::DragFloat3("position", position, 0.05);
    ImGui::DragFloat3("Rotation", rotation, 0.05);
    ImGui::DragFloat3("Scale", scale, 0.05);

    ImGui::Dummy(ImVec2(0, 10));

    pos = { position[0], position[1], position[2] };
    rot = { rotation[0], rotation[1], rotation[2] };
    sca = { scale[0], scale[1], scale[2] };

    comp.recompose(pos, rot, sca);
}

void ComponentEditorWindow::render_mesh() {
    auto comp = m_rendered->try_get<Mesh>();
    if (!comp) return;
    ImGui::Text("Mesh Component");
    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Text(comp->import_path.c_str());
    ImGui::Text(comp->material_path.c_str());

    ImGui::Dummy(ImVec2(0, 10));
}
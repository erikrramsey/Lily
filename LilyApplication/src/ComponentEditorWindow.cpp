#include "ComponentEditorWindow.h"

enum COMPONENTS {
    MESH = 0,
    CAMERA,
    LIGHT,
};

const char* COMPONENTSTR[] = {
        "Mesh",
        "Camera",
        "Light",
};

ComponentEditorWindow::ComponentEditorWindow(LilyEditor* parent) : EditorWindow(parent) {
    m_selected = nullptr;
    m_rendered = nullptr;
    m_file_explorer = nullptr;
    m_component_path.clear();
}

ComponentEditorWindow::~ComponentEditorWindow() {
    delete m_file_explorer;
    m_selected = nullptr;
    m_rendered = nullptr;
    m_file_explorer = nullptr;
    m_component_path.clear();
}

void ComponentEditorWindow::render() {
    m_rendered = m_parent->get_selected();
    if (!m_rendered) return;

    auto scene = m_parent->active_scene();
    if (m_file_explorer) {
        m_file_explorer->render();
        if (!m_component_path.empty()) {
            Importer::import_model(m_selected, m_component_path, m_parent->project_path());
            delete m_file_explorer;
            m_component_path.clear();
            m_file_explorer = nullptr;
        }
    }
    if (m_rendered->try_get<Camera>()) return;

    ImGui::Begin("Component Editor");

    ImGui::Text("%d", static_cast<int>(m_rendered->m_entity));
    ImGui::SameLine();
    const char* current = "Add Component";
    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton;
    if (ImGui::BeginCombo("##Add Component", current, flags)) {
        if (ImGui::Selectable(COMPONENTSTR[MESH])) {
            m_selected = m_rendered;
            m_file_explorer = new LilyFileExplorer("Select component resource", &m_component_path, m_parent->project_path());
        }
        if (ImGui::Selectable(COMPONENTSTR[CAMERA])) {}
        if (ImGui::Selectable(COMPONENTSTR[LIGHT])) {
            m_rendered->add_component<Light>();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    render_family();
    ImGui::Separator();
    render_transform();
    ImGui::Separator();
    render_mesh();
    ImGui::Separator();
    render_light();

    ImGui::End();
}

void ComponentEditorWindow::render_family() {
    auto& comp = m_rendered->get<Family>();
    ImGui::Text("Family Component");
    ImGui::Dummy(ImVec2(0, 10));

    ImGui::Text("Parent");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(comp.parent));

    auto scene = m_parent->active_scene();
    if (ImGui::Button("Set Root as Parent")) {
        scene->get_root()->add_child(m_rendered);
    }

    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton;
    const char* current = "Add Child";
    if (ImGui::BeginCombo("##Add Child", current, flags)) {
        for (auto& [ent, obj] : scene->m_objects) {
            bool is_valid = true;
            auto climb = m_rendered;
            while (climb != scene->get_root()) {
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

    ImGui::DragFloat3("position", position, 0.05f);
    ImGui::DragFloat3("Rotation", rotation, 0.05f);
    ImGui::DragFloat3("Scale", scale, 0.05f);

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

void ComponentEditorWindow::render_light() {
    auto comp = m_rendered->try_get<Light>();
    if (!comp) return;
    ImGui::Text("Light Component");
    ImGui::Dummy(ImVec2(0, 10));
}
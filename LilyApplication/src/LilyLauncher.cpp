#include "LilyLauncher.h"

using json = nlohmann::json;
LilyLauncher::LilyLauncher(LauncherData *data) : Gui() {
    m_explorer = nullptr;
    m_renaming = false;
    m_is_selecting_directory = false;
    m_selection_it = -1;
    m_data = data;
}

LilyLauncher::~LilyLauncher() {};

void LilyLauncher::init() {
    Gui::init();
    load_launcher_config();
}

void LilyLauncher::update(long long dt) {
    gui_render();
}

void LilyLauncher::gui_render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Application::get()->get_window().gWindow);
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags1 = ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags1 |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove;
    window_flags1 |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Lily Launcher", nullptr, window_flags1);
    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 0.1f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 5.0f);
    ImGui::BeginChild("Buttons", ImVec2(size.x * 0.3f, size.y));

    ImGui::Indent(5);
    ImGui::Dummy(ImVec2(0, 4));
    ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x - 10, 30);
    if (ImGui::Button("New Project", button_size)) {
        create_new_project();
    }

    if (ImGui::Button("Change Project Directory", button_size)) {
        select_directory();
    }

    if (ImGui::Button("Select Project", button_size)) {
        on_project_select();
    }

    if (ImGui::Button("Rename", button_size)) {
        if (!m_data->selected_project.empty()) {
            m_renaming = true;
        }
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Projects", ImVec2(0, size.y));
    if (!m_data->projects_directory.empty()) {
        int it = 0;
        for (auto& entry : fs::directory_iterator(m_data->projects_directory)) {
            std::string name = entry.path().filename().string();
            if (m_selection_it == it && m_renaming) {
                char input[80] = "";
                strcpy_s(input, entry.path().filename().string().c_str());
                ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText("##", input, 80, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                    auto newPath = fs::path(entry.path().parent_path().string() + "\\" + input);
                    fs::rename(entry.path(), newPath);
                    m_data->selected_project = newPath;
                    m_renaming = false;
                }
                else if (ImGui::IsItemDeactivated()) {
                    m_renaming = false;
                }
            } else {
                if (ImGui::Selectable(name.c_str(), m_selection_it == it, 0, ImVec2(ImGui::GetContentRegionAvail().x, 45))) {
                    m_data->selected_project = entry.path();
                    m_selection_it = it;
                }
                if (ImGui::IsMouseDoubleClicked(0)) {
                    on_project_select();
                }
            }
            it++;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);

    if (m_is_selecting_directory) {
        if (!m_data->projects_directory.empty()) {
            save_launcher_config();
            m_is_selecting_directory = false;
        }
    }

    if (m_explorer) {
        m_explorer->render();
        if (m_explorer->should_close()) {
            delete m_explorer;
            m_explorer = nullptr;
        }
    }

    ImGui::End();
    ImGui::EndFrame();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LilyLauncher::save_launcher_config() {
    std::ofstream outfile("LauncherConfig.json");
    json j;
    j["Projects Directory"] = m_data->projects_directory.string();
    outfile << j.dump(4);
    outfile.close();
}

void LilyLauncher::load_launcher_config() {
    std::ifstream infile("LauncherConfig.json");
    if (infile.good()) {
        json j;
        infile >> j;
        if (j.contains("Projects Directory") && j["Projects Directory"] != "") {
            m_data->projects_directory = static_cast<fs::path>(static_cast<std::string>((j["Projects Directory"])));
            return;
        }
    }

    select_directory();
}

void LilyLauncher::select_directory() {
    m_data->projects_directory.clear();
    m_explorer = new LilyFileExplorer("Select Project Directory", &m_data->projects_directory);
    m_is_selecting_directory = true;
}

void LilyLauncher::create_new_project() {
    int proj = 1;
    std::string name;
    fs::path new_proj;
    do {
        name = "\\New Project " + std::to_string(proj);
        proj++;
        new_proj = m_data->projects_directory.string() + name;
    } while (fs::exists(new_proj));
    fs::create_directory(new_proj);
    fs::create_directory(new_proj.string() + "\\Scenes");
}

void LilyLauncher::on_project_select() {
    if (!m_data->selected_project.empty()) {
        m_data->open_editor = true;
        SDL_Event event{ SDL_QUIT };
        SDL_PushEvent(&event);
    }
}
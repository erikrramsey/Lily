#include "LilyEditor.h"
#include <cstdlib>


LilyEditor::LilyEditor(LauncherData* data) : Gui() {
    m_data = data;
}

LilyEditor::~LilyEditor() { cleanup(); }

void LilyEditor::cleanup() {
    delete m_active_scene;
    delete m_framebuffer;
    delete m_scene_explorer;
    for (auto child : m_children) { delete child; }
    m_children.clear();

    m_active_scene = nullptr;
    m_framebuffer = nullptr;
    m_scene_explorer = nullptr;
}

void LilyEditor::init() {
    Gui::init();
	m_active_scene = new Scene();
	m_active_scene->Init();
	m_framebuffer = new Framebuffer(1920, 1080);
	m_framebuffer->Init();
    m_project_path = m_data->selected_project;

    m_scene_explorer = nullptr;
    m_children.push_back(new ComponentEditorWindow(this));
    m_children.push_back(new SceneListWindow(this));

    m_selected = nullptr;
}

void LilyEditor::update(long long dt) {
	Renderer::SetClearColor(glm::vec4(0.5, 0.5, 0.5, 1));
	Renderer::Clear();
	m_framebuffer->Bind();
	m_active_scene->update(dt);
	m_framebuffer->Unbind();


	if (m_forward_keydown && m_mouse_locked) {
        auto& cam = m_active_scene->get_camera().get<Camera>();
		cam.position += (cam.forward * ((float)dt / 1000) * cam.move_speed);
        cam.update();
	}

    gui_render();
}

void LilyEditor::OnEvent(SDL_Event& ev) {
	switch (ev.type) {
	case SDL_MOUSEMOTION:
		if (m_mouse_locked) {
            auto& cam = m_active_scene->get_camera().get<Camera>();
            cam.yaw_in(ev.motion.xrel);
            cam.pitch_in(ev.motion.yrel);
		}
		break;
	case SDL_KEYDOWN:
		switch (ev.key.keysym.sym) {
		case SDLK_ESCAPE:
			m_mouse_locked = !m_mouse_locked;
			break;
		case SDLK_w:
            m_forward_keydown = true;
		}
		break;
	case SDL_KEYUP:
		switch (ev.key.keysym.sym) {
		case SDLK_w:
            m_forward_keydown = false;
			break;
		}
		break;
	case SDL_WINDOWEVENT:
		if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
			std::cout << "LilyEditor: Resize event: "
			<< ev.window.data1 << ' '
			<< ev.window.data2 << std::endl;
		}
		break;
	}

	Gui::OnEvent(ev);
}

void LilyEditor::on_deserialize() {
    m_selected = nullptr;
}

void LilyEditor::gui_render() {
	static bool show_demo_window = true;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(Application::get()->get_window().gWindow);
	ImGui::NewFrame();


	ImGuiWindowFlags window_flags1 = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags1 |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags1 |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("Main Window", nullptr, window_flags1);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::ShowDemoWindow();

	bool opt_fullscreen = false;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Scene")) {
                SceneSerializer::serialize(m_active_scene, "TestScene.json");
            }
            if (ImGui::MenuItem("Load Scene")) {
                m_scene_explorer = new LilyFileExplorer("Select Scene (.json)", &m_current_scene_path);
                m_selected = nullptr;
            }
            if (ImGui::MenuItem("Clear")) {
                m_active_scene->clear();
                m_active_scene->Init();
                m_selected = nullptr;
            }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

    for (auto& win : m_children) {
        win->render();
    }

	entity_list_window();
	settings_window();

    if (m_scene_explorer) {
        m_scene_explorer->render();
        if (!m_current_scene_path.empty()) {
            SceneSerializer::deserialize(m_active_scene, m_current_scene_path.string());
            delete m_scene_explorer;
            m_scene_explorer = nullptr;
        }
    }

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;
	glm::vec2 a = m_framebuffer->GetResolution();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene View", 0, window_flags);
		auto window_x = ImGui::GetContentRegionAvail().x;
		auto fb_x = m_framebuffer->GetResolution().x;
		if (window_x != fb_x) {
			auto y = window_x * 9 / 16.0;
			m_active_scene->get_camera().get<Camera>().Initialize(window_x, y);
			m_framebuffer->Resize(window_x, y);
		}
		ImGui::Image(reinterpret_cast<void*>(m_framebuffer->GetTextureAttachment()), ImVec2{ a.x, a.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End(); // Scene View
	ImGui::PopStyleVar();

	ImGui::End(); // Main window
	ImGui::PopStyleVar(2);
	ImGui::EndFrame();
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LilyEditor::display_Lobject(Lobject* obj) {
	std::string name = std::string(obj->get_name());
	
	if (m_renaming && obj == m_selected) {
		char input[80] = "";
		strcpy_s(input, m_selected->get_name());
		ImGui::SetKeyboardFocusHere();
		if (ImGui::InputText("##", input, 80, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
			obj->set_name(input);
            m_renaming = false;
		}
		else if (ImGui::IsItemDeactivated()) {
            m_renaming = false;
		}
		return;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	bool open = ImGui::TreeNodeEx(name.c_str(), flags);
	if (ImGui::IsItemClicked()) {
		m_selected = obj;
		if (ImGui::IsMouseDoubleClicked(0)) {
            m_renaming = true;
		}
	}
	if (open) {
		for (auto it : obj->get_children()) {
			display_Lobject(it);
		}
		ImGui::TreePop();
	}
}

void LilyEditor::entity_list_window() {
    ImGui::Begin("Lobject List");
	if (ImGui::Button("Create New Entity")) {
		m_active_scene->create_Lobject();
	}
    if (ImGui::Button("Delete Selected")) {
        m_active_scene->delete_Lobject(m_selected);
        m_selected = nullptr;
    }

    for (auto& obj : m_active_scene->get_root()->get_children())
        display_Lobject(obj);

	ImGui::End(); // Entity List
}

void LilyEditor::settings_window() {
	ImGui::Begin("Settings");
	if (ImGui::SliderInt("V-Sync", &m_vsync, -1, 1))
        Application::get()->get_window().set_vsync(m_vsync);
	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End(); //Settings
}
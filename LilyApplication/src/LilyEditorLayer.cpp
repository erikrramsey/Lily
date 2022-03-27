#include "LilyEditor.h"
#include <cstdlib>

LilyEditorLayer::~LilyEditorLayer() {
	delete m_active_scene;
	delete m_framebuffer;
	delete m_file_explorer;
}

void LilyEditorLayer::Init() {
	GuiLayer::Init();
	m_app = Application::Get();
	m_active_scene = new Scene();
	m_active_scene->Init();
	m_active_camera = &m_active_scene->getCamera().get<Camera>();
	m_active_camera->Initialize(1920, 1080);

	selected = nullptr;
	m_framebuffer = new Framebuffer(1920, 1080);
	m_framebuffer->Init();

	m_file_explorer = new LilyFileExplorer;
}


void LilyEditorLayer::Update(long long dt) {
	Renderer::SetClearColor(glm::vec4(0.5, 0.5, 0.5, 1));
	Renderer::Clear();
	m_framebuffer->Bind();
	m_active_scene->update(dt);
	m_framebuffer->Unbind();

	if (forward_keydown && m_mouse_locked) {
		m_active_camera->Position += (m_active_camera->Forward * ((float)dt / 1000) * m_active_camera->MoveSpeed);
		m_active_camera->Update();
	}

	GuiRender();
}

void LilyEditorLayer::OnEvent(SDL_Event& ev) {
	switch (ev.type) {
	case SDL_MOUSEMOTION:
		if (m_mouse_locked) {
			m_active_camera->YawIn(ev.motion.xrel);
			m_active_camera->PitchIn(ev.motion.yrel);
		}
		break;
	case SDL_KEYDOWN:
		switch (ev.key.keysym.sym) {
		case SDLK_ESCAPE:
			m_mouse_locked = !m_mouse_locked;
			break;
		case SDLK_w:
			forward_keydown = true;
		}
		break;
	case SDL_KEYUP:
		switch (ev.key.keysym.sym) {
		case SDLK_w:
			forward_keydown = false;
			break;
		}
		break;
	case SDL_WINDOWEVENT:
		if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
			std::cout << "LilyEditorayer: Resize event: "
			<< ev.window.data1 << ' '
			<< ev.window.data2 << std::endl;
		}
		break;
	}

	GuiLayer::OnEvent(ev);
}

void LilyEditorLayer::GuiRender() {
	static bool show_demo_window = true;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_app->GetWindow().gWindow);
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
			if (ImGui::MenuItem("Load Mesh")) m_show_file_explorer = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Begin("UObject Editor");
	entity_editor_window();
	ImGui::End(); // Entity Editor
	entity_list_window();
	settings_window();

	if (m_show_file_explorer) {
		m_file_explorer->render();
		auto p = m_file_explorer->get_selection();
		if (!p.empty()) {
			m_active_scene->load(p.string());
			m_show_file_explorer = false;
		}
	}


	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground;
	glm::vec2 a = m_framebuffer->GetResolution();
	ImGui::SetNextWindowSize(ImVec2{ a.x, a.y });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene View", 0, window_flags);
		
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

void LilyEditorLayer::entity_editor_window() {

	if (selected == nullptr || selected->try_get<Camera>()) return;

	ImGui::Text("%d", selected->m_entity);

	Transform* trans = &selected->get<Transform>();
	glm::vec3 pos, rot, sca;

	trans->decompose(pos, rot, sca);

	ImGui::DragFloat("Position X", &pos.x, 0.05f);
	ImGui::DragFloat("Position Y", &pos.y, 0.05f);
	ImGui::DragFloat("Position Z", &pos.z, 0.05f);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::SliderFloat("Rotation X", &rot.x, 0.0f, 2.0 * AI_MATH_PI);
	ImGui::SliderFloat("Rotation Y", &rot.y, 0.0f, 2.0 * AI_MATH_PI);
	ImGui::SliderFloat("Rotation Z", &rot.z, 0.0f, 2.0 * AI_MATH_PI);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::DragFloat("Scale X", &sca.x, 0.01f);
	ImGui::DragFloat("Scale Y", &sca.y, 0.01f);
	ImGui::DragFloat("Scale Z", &sca.z, 0.01f);

	trans->recompose(pos, rot, sca);

	if (ImGui::Button("Delete Lobject")) {
		m_active_scene->delete_Lobject(selected);
		selected = nullptr;
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

}

void LilyEditorLayer::display_Lobject(Lobject* obj) {
	static bool renaming = false;
	std::string nodename = std::string(obj->get_name());
	
	if (renaming && obj == selected) {
		char input[80] = "";
		strcpy(input, selected->get_name());
		ImGui::SetKeyboardFocusHere();
		if (ImGui::InputText("##", input, 80, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
			obj->set_name(input);
			renaming = false;
		}
		else if (ImGui::IsItemDeactivated()) {
			renaming = false;
		}
		return;
	}
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	bool open = ImGui::TreeNodeEx(nodename.c_str(), flags);
	if (ImGui::IsItemClicked()) {
		selected = obj;
		if (ImGui::IsMouseDoubleClicked(0)) {
			renaming = true;
		}
	}
	if (open) {
		for (auto it : obj->get_children()) {
			display_Lobject(it);
		}
		ImGui::TreePop();
	}
}

void LilyEditorLayer::entity_list_window() {
	ImGui::Begin("Lobject List");
	for (auto& [ent, obj] : m_active_scene->m_objects) {
		auto a = obj->get<Family>();
		if (a.parent == 0) {
			display_Lobject(obj);
		}
	}

	ImGui::End(); // Entity List
}

void LilyEditorLayer::settings_window() {
	ImGui::Begin("Settings");
	if (ImGui::SliderInt("V-Sync", &vsync, -1, 1))
		m_app->GetWindow().set_vsync(vsync);
	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End(); //Settings
}

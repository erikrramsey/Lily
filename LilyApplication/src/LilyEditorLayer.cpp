#include "LilyEditor.h"
#include <cstdlib>

LilyEditorLayer::~LilyEditorLayer() {}

void LilyEditorLayer::Init() {
	GuiLayer::Init();
	m_app = Application::Get();
	m_active_scene = new Scene();
	m_active_scene->Init();
	m_active_camera = &m_active_scene->getCamera();
	m_active_camera->Initialize(1280, 720);


	selected = nullptr;
	m_framebuffer = new Framebuffer(1280, 720);
	m_framebuffer->Init();

	// make importer return one big mesh object when importing model from filepath
	auto model_path = "../../LilyApplication/models/objects/UgwasadStore/UgwasadStore.obj";
	Importer model(model_path);
	auto obj = m_active_scene->create_UObject();

	//for (int i = 0; i < model.meshes.size(); i++) {
		Mesh_component mesh(model.meshes[0], obj->get_component<Transform_component>());
		m = &obj->add_component(mesh);
	//}
}


void LilyEditorLayer::Update(long long dt) {
	Renderer::SetClearColor(glm::vec4(0.5, 0.5, 0.5, 1));
	Renderer::Clear();
	m_framebuffer->Bind();
	m_active_scene->Update(dt);
	m_framebuffer->Unbind();

	if (forward_keydown && m_mouse_locked) {
		m_active_camera->Translate(FORWARD, dt);
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
		std::cout << ev.window.data1 << std::endl;
		std::cout << ev.window.data2 << std::endl;
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
	ImGui::ShowDemoWindow();
	ImGuiIO& io = ImGui::GetIO();
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	bool opt_fullscreen = false;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Begin("Entity Editor");
	RenderEntityEditorWindow();
	ImGui::End();
	RenderEntityListWindow();
	ImGui::End();
	ImGui::PopStyleVar(2);


	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground;
	glm::vec2 a = m_framebuffer->GetResolution();
	ImGui::SetNextWindowSize(ImVec2{ a.x, a.y });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Scene View", 0, window_flags);
		
	ImGui::Image(reinterpret_cast<void*>(m_framebuffer->GetTextureAttachment()), ImVec2{ a.x, a.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LilyEditorLayer::RenderEntityEditorWindow() {

	if (selected == nullptr) return;

	Transform_component& trans = selected->get_component<Transform_component>();
	Mesh_component* mesh = selected->try_get_component<Mesh_component>();

	ImGui::Text("%d", selected->m_id);
	if (!mesh) return;

	auto vec = trans.GetPosition();
	ImGui::DragFloat("Position X", &vec.x, 0.05f);
	ImGui::DragFloat("Position Y", &vec.y, 0.05f);
	ImGui::DragFloat("Position Z", &vec.z, 0.05f);
	trans.SetPosition(vec);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	vec = trans.GetRotation();
	ImGui::SliderFloat("Rotation X", &vec.x, 0.0f, 2.0 * AI_MATH_PI);
	ImGui::SliderFloat("Rotation Y", &vec.y, 0.0f, 2.0 * AI_MATH_PI);
	ImGui::SliderFloat("Rotation Z", &vec.z, 0.0f, 2.0 * AI_MATH_PI);
	trans.SetRotation(vec);

	mesh->basis = trans.GetBasis();

	if (ImGui::Button("Delete Entity")) {
		selected->remove_component<Mesh_component>();
		selected = nullptr;
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void LilyEditorLayer::RenderEntityListWindow() {
	ImGui::Begin("Entity List");
	if (ImGui::Button("New Entity")) {
		auto obj = m_active_scene->create_UObject();
		auto& t = obj->get_component<Transform_component>();
		obj->add_component<Mesh_component>(selected->get_component<Mesh_component>());
		obj->get_component<Mesh_component>().basis = t.GetBasis();
	}

	auto it = m_active_scene->m_objects.begin();

	while (it != m_active_scene->m_objects.end()) {
		auto a = (*it)->GetName();
		bool is_selected = (*it == selected);
		if (ImGui::Selectable(a, is_selected)) {
			selected = *it;
		}
		it++;
	}

	ImGui::End();
}

#pragma once
#include "Lily.h"
#include "Renderer/Framebuffer.h"

#include "LilyFileExplorer.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <filesystem>

using namespace Lily;

class LilyEditorLayer : public GuiLayer {
public:
    LilyEditorLayer() : GuiLayer() {};
	~LilyEditorLayer();
	void Update(long long dt) override;
	void Init() override;

	void OnEvent(SDL_Event& event) override;
private:
	void GuiRender();

	void entity_editor_window();
	void entity_list_window();
	void settings_window();

	void display_Lobject(Lobject* obj);

	Camera* m_active_camera;

	Lobject* selected;

	Scene* m_active_scene;
	Shader* m_current_shader;
	Framebuffer* m_framebuffer;

	LilyFileExplorer* m_file_explorer;
	bool m_mouse_locked = false;
	bool forward_keydown = false;
	bool m_show_file_explorer = false;
	int vsync = 1;
};

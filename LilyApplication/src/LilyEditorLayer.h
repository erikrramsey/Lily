#pragma once
#include "Lily.h"
#include "Renderer/Framebuffer.h"

#include "LilyFileExplorer.h"
#include "ComponentEditorWindow.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <filesystem>

using namespace Lily;

class ComponentEditorWindow;
class LilyEditorLayer : public GuiLayer {
public:
    LilyEditorLayer() : GuiLayer() {};
	~LilyEditorLayer();
	void Update(long long dt) override;
	void Init() override;

	void OnEvent(SDL_Event& event) override;
private:
	void GuiRender();
	void entity_list_window();
	void settings_window();
	void display_Lobject(Lobject* obj);

    ComponentEditorWindow* m_component_editor;

	Camera* m_active_camera;
	Lobject* selected;
	Scene* m_active_scene;
	Framebuffer* m_framebuffer;
    LilyFileExplorer* m_scene_explorer;

	bool m_mouse_locked = false;
	bool forward_keydown = false;
	bool renaming = false;
	int vsync = 1;

    friend class ComponentEditorWindow;
};

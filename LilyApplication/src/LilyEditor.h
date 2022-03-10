#pragma once
#include "Lily.h"
#include "Renderer/Framebuffer.h"

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
	void LilyEditorLayer::RenderEntityEditorWindow();
	void LilyEditorLayer::RenderEntityListWindow();

	Camera* m_active_camera;

	bool m_mouse_locked = false;
	bool forward_keydown = false;

	Mesh_component* m;

	UObject* selected;

	Scene* m_active_scene;
	Shader* m_current_shader;
	Framebuffer* m_framebuffer;
};

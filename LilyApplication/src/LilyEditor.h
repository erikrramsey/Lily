#pragma once
#include "Lily.h"
#include "Renderer/Framebuffer.h"

#include "EditorWindow.h"
#include "ComponentEditorWindow.h"
#include "RendererSettingsWindow.h"
#include "SceneListWindow.h"
#include "LilyFileExplorer.h"
#include "LauncherData.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <filesystem>
#include <vector>

using namespace Lily;

class ComponentEditorWindow;
class LilyEditor : public Gui {
public:
    LilyEditor(LauncherData* data);
	~LilyEditor();
	void update(long long dt) override;
	void init() override;
    void cleanup();


    void on_deserialize();
    Lobject* get_selected() { return m_selected; }

	void OnEvent(SDL_Event& event) override;

    const fs::path& project_path() { return m_project_path; }
    Scene* active_scene() { return m_active_scene; }
private:
	void gui_render();
	void entity_list_window();
	void settings_window();
	void display_Lobject(Lobject* obj);

    LauncherData* m_data;
    fs::path m_current_scene_path;
    fs::path m_project_path;

	Scene* m_active_scene;
	Framebuffer* m_framebuffer;
    LilyFileExplorer* m_scene_explorer;
    Lobject* m_selected;

	bool m_mouse_locked = false;
	bool m_forward_keydown = false;
	bool m_renaming = false;
	int m_vsync = 1;

    std::vector<EditorWindow*> m_children;

    friend class ComponentEditorWindow;
};

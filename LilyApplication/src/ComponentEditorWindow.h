#pragma once

#include "Lily.h"
#include "LilyFileExplorer.h"
#include "LilyEditor.h"
#include "EditorWindow.h"

#include <filesystem>
namespace fs = std::filesystem;

using namespace Lily;

class LilyEditor;
class ComponentEditorWindow : public EditorWindow {
public:
    explicit ComponentEditorWindow(LilyEditor* parent);
    void render() override;
private:
    void render_transform();
    void render_family();
    void render_mesh();

    LilyFileExplorer* m_file_explorer;
    fs::path m_component_path;

    Scene* m_scene;

    Lobject* m_rendered;
    Lobject* m_selected;
};
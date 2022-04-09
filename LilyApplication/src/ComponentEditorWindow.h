#pragma once

#include "Lily.h"
#include "LilyFileExplorer.h"
#include "LilyEditorLayer.h"

using namespace Lily;

class LilyEditorLayer;
class ComponentEditorWindow {
public:
    explicit ComponentEditorWindow(LilyEditorLayer* parent);

    void render_Lobject(Lobject* obj);
    void render_Lobject();

private:
    void render();
    void render_transform();
    void render_family();
    void render_mesh();

    LilyEditorLayer* m_parent;
    LilyFileExplorer* m_file_explorer;

    Lobject* m_rendered;
    Lobject* m_selected;
    bool m_show_file_explorer;
};
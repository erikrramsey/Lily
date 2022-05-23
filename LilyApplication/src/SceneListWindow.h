#pragma once

#include "EditorWindow.h"
#include "LilyEditor.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;
class SceneListWindow : public EditorWindow {
public:
    explicit SceneListWindow(LilyEditor* parent);
    ~SceneListWindow() override;
    void render() override;
private:
    int m_selection_it;
    fs::path m_scenes_directory;
    fs::path m_selected;
    bool m_renaming;
};
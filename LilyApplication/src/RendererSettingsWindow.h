//
// Created by Erik on 6/27/2022.
//
#pragma once

#include <filesystem>

#include "Lily.h"
#include "LilyEditor.h"
#include "EditorWindow.h"
#include "Renderer/Renderer.h"

namespace fs = std::filesystem;
class RendererSettingsWindow : public EditorWindow {
public:
    explicit RendererSettingsWindow(LilyEditor* parent);
    void render() override;
private:
    void shader_list();
    fs::path m_shader_directory;
};

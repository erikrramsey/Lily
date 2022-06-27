//
// Created by Erik on 6/27/2022.
//
#pragma once

#include "Lily.h"
#include "EditorWindow.h"

class RendererSettingsWindow : public EditorWindow {
public:
    explicit RendererSettingsWindow(LilyEditor* parent) : EditorWindow(parent) {}
    void render() override;
private:
};

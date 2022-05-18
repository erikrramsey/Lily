#pragma once

#include "EditorWindow.h"
#include "json.hpp"

#include <imgui.h>
#include <filesystem>
namespace fs = std::filesystem;

class LilyFileExplorer {
public:
	LilyFileExplorer(const char* title, fs::path* path);
    LilyFileExplorer(const char* title, fs::path* path, fs::path current);
    ~LilyFileExplorer();
	void render();
    bool should_close() const { return !m_selection->empty() || m_should_close; }
private:
    const char* m_title;
	fs::path m_current_dir;
	fs::path* m_selection;
    int m_selection_it;
    bool m_should_close;
};
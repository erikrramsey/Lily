#include "LilyFileExplorer.h"

LilyFileExplorer::LilyFileExplorer(const char* title, fs::path* path) {
    m_title = title;
    m_selection = path;
	m_current_dir = fs::current_path().root_path();
    m_selection_it = -1;
    m_should_close = false;
}

LilyFileExplorer::LilyFileExplorer(const char* title, fs::path* path, fs::path current) {
    m_title = title;
    m_selection = path;
    m_current_dir = std::move(current);
    m_selection_it = -1;
    m_should_close = false;
}

LilyFileExplorer::~LilyFileExplorer() = default;

void LilyFileExplorer::render() {
    bool is_open;
	ImGui::Begin(m_title, &is_open);

    m_should_close = !is_open;

    auto selected = ImGui::Button("Select directory");
	ImGui::Selectable("..");
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
		m_current_dir = m_current_dir.parent_path();
		ImGui::End();
		return;
	}

    int it = 0;
	for (const auto& entry : std::filesystem::directory_iterator(m_current_dir)) {
		if (ImGui::Selectable(entry.path().string().c_str(), it == m_selection_it)) {
            m_selection_it = it;
        }
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			if (fs::is_directory(entry.path())) {
				m_current_dir = entry;
			} else {
				*m_selection = entry.path();
			}
		}
        if (selected && it == m_selection_it) {
            *m_selection = entry.path();
            selected = false;
        }
        it++;
	}

    if (selected) {
        *m_selection = m_current_dir;
    }

	ImGui::End();
}
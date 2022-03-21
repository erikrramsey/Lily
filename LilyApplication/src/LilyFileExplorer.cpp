#include "LilyFileExplorer.h"

LilyFileExplorer::LilyFileExplorer() {
	m_current_dir = fs::current_path();
}

void LilyFileExplorer::render() {
	ImGui::Begin("File Explorer");
	ImGui::Selectable("..");
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
		m_current_dir = m_current_dir.parent_path();
		ImGui::End();
		return;
	}
	for (const auto& entry : std::filesystem::directory_iterator(m_current_dir)) {
		ImGui::Selectable(entry.path().string().c_str());
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			if (fs::is_directory(entry.path())) {
				m_current_dir = entry;
			} else {
				m_selection = entry.path();
			}
		}
	}
	ImGui::End();
}

fs::path LilyFileExplorer::get_selection() {
	auto val = m_selection;
	m_selection.clear();
	return val;
}

#include <imgui.h>
#include <filesystem>
namespace fs = std::filesystem;

class LilyFileExplorer {
public:
	LilyFileExplorer();
	void render();
	fs::path get_selection();
private:
	fs::path m_current_dir;
	fs::path m_selection;
};
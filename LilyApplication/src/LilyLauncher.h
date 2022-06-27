#pragma once

#include "Lily.h"
#include "LilyFileExplorer.h"
#include "LauncherData.h"

#include <filesystem>
#include <fstream>

using namespace Lily;
namespace fs = std::filesystem;
class LilyLauncher : public Gui {
public:
    LilyLauncher(LauncherData* data);
    ~LilyLauncher();
    void update(long long dt) override;
    void init() override;

private:
    LauncherData* m_data;
    LilyFileExplorer* m_explorer;
    void gui_render();


    int m_selection_it;
    bool m_renaming;
    bool m_is_selecting_directory;

    void select_directory();
    void create_new_project();

    void save_launcher_config();
    void load_launcher_config();
    void on_project_select();
};
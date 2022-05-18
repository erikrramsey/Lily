#pragma once

struct LauncherData {
    bool open_editor = false;
    fs::path projects_directory;
    fs::path selected_project;
};

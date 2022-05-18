#include "LilyEditor.h"
#include "LilyLauncher.h"

#include <filesystem>

Application* Application::s_instance;
namespace fs = std::filesystem;


class LilyLauncherApp : public Application {
public:
    LilyLauncherApp(LauncherData* data) : Application() {
        m_window_size[0] = 720;
        m_window_size[1] = 480;
        m_layer = new LilyLauncher(data);
        AddLayer(m_layer);
        Init("Lily Launcher");
    }

    ~LilyLauncherApp() {
        delete m_layer;
    }

    LilyLauncher* m_layer;
};

class LilyApp : public Application {
public:
	LilyApp(LauncherData* data) : Application() {
		m_layer = new LilyEditor(data);
		AddLayer(m_layer);
		Init();
	};
	~LilyApp();
    
private:
	LilyEditor* m_layer;
};

LilyApp::~LilyApp() {
	delete m_layer;
}

int main(int argc, char* argv[]) {
    LauncherData data;
    auto launcher = new LilyLauncherApp(&data);
    launcher->Run();
    delete launcher;

    if (data.open_editor) {
        auto app = new LilyApp(&data);
        app->Run();
        delete app;
    }
	return 0;
}

#include "LilyEditorLayer.h"

Application* Application::s_instance;

class LilyEditor : public Application {
public:
	LilyEditor() : Application() {
		m_layer = new LilyEditorLayer();
		AddLayer(m_layer);
		Init();
	};
	~LilyEditor();
    
private:
	LilyEditorLayer* m_layer;
};

LilyEditor::~LilyEditor() {
	delete m_layer;
}

int main(int argc, char* argv[]) {
	auto app = new LilyEditor();
	app->Run();
	delete app;
	return 0;
}

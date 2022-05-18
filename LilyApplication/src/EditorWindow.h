#pragma once

class LilyEditor;
class EditorWindow {
public:
    explicit EditorWindow(LilyEditor* parent) {
        m_parent = parent;
    }
    virtual void render() = 0;
protected:
    LilyEditor* m_parent;
};

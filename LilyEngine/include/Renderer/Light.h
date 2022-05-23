#pragma once
#include "include/Entity/Component.h"

namespace Lily {

class Light : public Component {
public:
    Light(Entity self) : Component(self) , m_position(0.0f) {}
    Light(Entity self, glm::vec3 position) : Component(self) { m_position = position; }
    glm::vec3 get_pos() { return m_position; }
    void set_pos(glm::vec3 pos) { m_position = pos; }
private:
    glm::vec3 m_position;
};

}
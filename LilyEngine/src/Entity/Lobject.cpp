#include "Entity/Lobject.h"

namespace Lily {

Lobject::Lobject(Entity id, Scene* scene) {
	m_entity = id;
	m_scene = scene;
	m_name = "Object #" + std::to_string(id);
}

Lobject::~Lobject() {}
}
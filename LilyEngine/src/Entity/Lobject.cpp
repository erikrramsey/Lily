#include "Entity/Lobject.h"

namespace Lily {

Lobject::Lobject(Entity id, Scene* scene) {
	m_entity = id;
	m_scene = scene;
	m_name = "Object #" + std::to_string(id);
}

Lobject::~Lobject() {}

std::vector<Lobject*> Lobject::get_children() {
	std::vector<Lobject*> children;
	for (auto i : get<Family>().children) {
		children.push_back(m_scene->get(i));
	}
	return children;
}



}
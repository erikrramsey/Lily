#include "Entity/UObject.h"

namespace Lily {

	// replace this with a scene based constructor
	UObject::UObject(Entity id, Scene* scene) {
		m_parent = NULL;
		m_id = id;
		m_scene = scene;
		m_name = "Object #" + std::to_string(id);
	}

	UObject::UObject(Entity id, UObject& parent) {
		m_parent = &parent;
		m_id = id;
		m_scene = parent.m_scene;
		parent.add_child(this);
	}

	UObject::~UObject() {}

	UObject* UObject::add_child(UObject* child) {
		m_children.push_back(child);
		return child;
	}
}
#pragma once
#include "Component.h"
#include "Entity.h"

#include <vector>

namespace Lily {

struct Family : public Component {
public:
	Family(Entity self) : Component(self) {
		parent = 0;
	}

	void set_parent(Family& other) {
		parent = other.get_ent();
		other.children.push_back(get_ent());
	}

	Entity parent;
	std::vector<Entity> children;
};

}
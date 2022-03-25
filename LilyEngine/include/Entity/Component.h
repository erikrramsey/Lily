#pragma once

#include <vector>

#include "Entity/Entity.h"
#include "Renderer/Graphics_headers.h"

namespace Lily {

struct Component {
public:
	Component(Entity self) {
		m_self = self;
	}

	Entity get_ent() { return m_self; }

private:
	Entity m_self;
};

}

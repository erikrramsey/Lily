#include "Entity/EC_registry.h"

namespace Lily {

	EC_registry::EC_registry() {

	}

	EC_registry::~EC_registry() {

	}

	Entity EC_registry::create() {
		m_ents.emplace(m_next_id++);
		return m_next_id - 1;
	}


    void EC_registry::Delete_Entity(Entity ent) {
        
    }
}
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

struct Transform : public Component {
	Transform(Entity self, Entity parent = 0,
	glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1)) : Component(self) {
		m_parent = parent;
		m_local_basis = glm::mat4(1.0);
		m_world_basis = glm::mat4(1.0);
		UpdateBasis(pos, rot, sca);
	}

	void UpdateBasis(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1)) {
		m_pos = pos;
		m_rot = rot;
		m_sca = sca;
	}

	void set_position(glm::vec3& pos) {
		m_local_basis = glm::translate(glm::mat4(1.0), pos);
		m_world_basis = m_local_basis;
	}


	void decompose(glm::vec3& pos, glm::vec3& rot, glm::vec3& sca) {
		pos = m_pos;
		rot = m_rot;
		sca = m_sca;
	}

	const glm::mat4& get_worldspace()    { return m_world_basis; }
	void set_worldspace(glm::mat4& worldspace) { m_world_basis = worldspace; }

	const glm::mat4& get_localspace()    { return m_local_basis; }
	void set_localspace(glm::mat4& localspace) { m_local_basis = localspace; }

	Entity get_parent() { return m_parent; }
	void set_parent(Entity parent) { m_parent = parent; }

	glm::vec3& get_pos() { return m_pos; }
	void set_pos(glm::vec3& pos) { m_pos = pos; }

	glm::vec3& get_rot() { return m_rot; }
	void set_rot(glm::vec3& rot) { m_rot = rot; }

	glm::vec3& get_sca() { return m_rot; }
	void set_sca(glm::vec3& sca) { m_sca = sca; }

private:
	glm::mat4 m_world_basis;
	glm::mat4 m_local_basis;

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_sca;

	Entity m_parent;
};

}

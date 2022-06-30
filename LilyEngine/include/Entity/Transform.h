#pragma once

#include "Component.h"
#include "Renderer/GraphicsHeaders.h"

namespace Lily {

struct Transform : public Component {
	Transform(Entity self, glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1)) : Component(self) {
		m_local_basis = glm::mat4(1.0);
		m_world_basis = glm::mat4(1.0);
		recompose(pos, rot, sca);
	}

	void decompose(glm::vec3& pos, glm::vec3& rot, glm::vec3& sca) {
		pos = m_pos;
		rot = m_rot;
		sca = m_sca;
	}

	void recompose(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1)) {
		m_pos = pos;
		m_rot = rot;
		m_sca = sca;
	}

	const glm::mat4& get_worldspace() { return m_world_basis; }
	void set_worldspace(glm::mat4& worldspace) { m_world_basis = worldspace; }

	const glm::mat4& get_localspace() { return m_local_basis; }
	void set_localspace(glm::mat4& localspace) { m_local_basis = localspace; }

	glm::vec3& get_pos() { return m_pos; }
	void set_pos(glm::vec3& pos) { m_pos = pos; }

	glm::vec3& get_rot() { return m_rot; }
	void set_rot(glm::vec3& rot) { m_rot = rot; }

	glm::vec3& get_sca() { return m_sca; }
	void set_sca(glm::vec3& sca) { m_sca = sca; }

private:
	glm::mat4 m_world_basis;
	glm::mat4 m_local_basis;

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_sca;
};

}

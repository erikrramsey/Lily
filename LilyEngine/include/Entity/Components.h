#pragma once

#include <glm/glm.hpp>

#include "Renderer/Graphics_headers.h"
#include "Renderer/Mesh.h"

namespace Lily {

struct Transform_component {
	Transform_component(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1)) {
		Position = pos;
		Rotation = rot;
		Scale = sca;
		UpdateBasis();
	}

	void UpdateBasis() {
		Basis = glm::translate(glm::mat4(1.0), Position);

		Basis = glm::rotate(Basis, Rotation.x, glm::vec3(1.0, 0.0, 0.0));
		Basis = glm::rotate(Basis, Rotation.y, glm::vec3(0.0, 1.0, 0.0));
		Basis = glm::rotate(Basis, Rotation.z, glm::vec3(0.0, 0.0, 1.0));
	}

	void SetPosition(glm::vec3& pos) {
		Position = pos;
		UpdateBasis();
	}

	void SetRotation(glm::vec3& rot) {
		Rotation = rot;
		UpdateBasis();
	}

	void SetScale(glm::vec3& sca) {
		Scale = sca;
		UpdateBasis();
	}

	const glm::vec3& GetPosition() { return Position; }
	const glm::vec3& GetRotation() { return Rotation; }
	const glm::vec3& GetScale()    { return Scale; }
	const glm::mat4& GetBasis()    { return Basis; }

private:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::mat4 Basis;
};

struct Camera_component {
	Camera_component() {
		fov = 90;
		near_plane = 100;
		far_plane = 1000;
	}

	Transform_component position = Transform_component(glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0));

	int fov;
	int near_plane;
	int far_plane;
};

struct Mesh_component {
	Mesh_component(Mesh& m, Transform_component& trans) : mesh(m) {
		basis = trans.GetBasis();
	}

	Mesh_component(const Mesh_component& other, Transform_component& trans) : mesh(other.mesh) {
		basis = trans.GetBasis();
	}

	Mesh mesh;
	glm::mat4 basis;

private:
	Mesh_component();
};

}

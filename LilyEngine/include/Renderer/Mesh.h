#pragma once

#include <vector>

#include "Renderer/Graphics_headers.h"
#include "Renderer/Shader.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"

namespace Lily {
struct Mesh : public Component {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
    std::string name;

	Mesh(Entity self, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(Entity self, const Mesh& other);

	unsigned int VAO, VBO, EBO;

private:
	Mesh();
	void initMesh();
};

}
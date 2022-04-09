#pragma once

#include <vector>
#include <filesystem>

#include "Renderer/Graphics_headers.h"
#include "Renderer/Shader.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"

namespace fs = std::filesystem;

namespace Lily {
struct Mesh : public Component {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
    std::string name;

    std::string import_path;
    std::string material_path;

	Mesh(Entity self, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(Entity self, const Mesh& other);
    Mesh(Entity self, const std::string& import_path);
    void initMesh();

	unsigned int VAO, VBO, EBO;
    bool imported;

private:
	Mesh();
};

}
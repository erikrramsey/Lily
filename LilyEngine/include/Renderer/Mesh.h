#pragma once

#include <vector>
#include <filesystem>

#include "Renderer/GraphicsHeaders.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"

namespace fs = std::filesystem;

namespace Lily {
struct Mesh : public Component {
public:
    Material material;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
    std::string name;

    std::string import_path;
    std::string material_path;

	Mesh(Entity self, std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material);
	Mesh(Entity self, const Mesh& other);
    Mesh(Entity self, const std::string& import_path);
    void initMesh();

	unsigned int VAO, VBO, EBO;
    bool imported;

private:
	Mesh();
};

}
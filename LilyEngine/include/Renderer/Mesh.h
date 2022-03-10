#pragma once

#include <vector>

#include "Renderer/Graphics_headers.h"
#include "Renderer/Shader.h"

namespace Lily {
    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(const Mesh& other);

        unsigned int VAO, VBO, EBO;

    private:
        Mesh();
        void initMesh();

    };
}
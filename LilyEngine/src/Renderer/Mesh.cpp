#include <string>

#include "Renderer/Mesh.h"

namespace Lily {

    Mesh::Mesh(Entity self, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : Component(self) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        initMesh();
    }

    Mesh::Mesh(Entity self, const Mesh& other) : Component(self) {
        vertices = other.vertices;
        indices = other.indices;
        textures = other.textures;

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
    }

    Mesh::Mesh(Entity self, const std::string& _import_path) : Component(self) {
        VAO = 0;
        VBO = 0;
        EBO = 0;
        import_path = std::move(_import_path);
        imported = false;
    }

    void Mesh::initMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
}
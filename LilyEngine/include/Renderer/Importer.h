#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Graphics_headers.h"

namespace Lily {
    class Importer {
    public:
        Importer(std::string path);
        std::vector<Mesh> meshes;
    private:
        std::string directory;
        std::vector<Texture> textures_loaded;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // issue with the file path string, hard coded to the current nanosuit directory to test texture loading
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };
}

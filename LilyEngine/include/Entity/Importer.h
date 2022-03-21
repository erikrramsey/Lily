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
#include "Renderer/Renderer.h"
#include "Renderer/stb_image.h"

#include "Entity/Lobject.h"

namespace Lily {
class Scene;
class Importer {
public:
	Importer(Scene* scene);
	void load_model(std::string& path);

private:
	Scene* m_scene;
	std::string directory;
	std::vector<Texture> textures_loaded;
	Entity parent;

	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
}

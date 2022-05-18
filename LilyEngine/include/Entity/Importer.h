#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
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
class Lobject;
class Importer {
public:
	Importer();
	static void import_model(Lobject* obj, std::string& path);
    static void import_sub_mesh(Mesh& lmesh);

private:
	Scene* m_scene;
	std::string directory;
	std::vector<Texture> textures_loaded;
	Lobject* parent;
};
}

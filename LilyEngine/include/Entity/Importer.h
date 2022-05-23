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

namespace fs = std::filesystem;
namespace Lily {
class Scene;
class Lobject;
class Importer {
public:
	Importer();
	static void import_model(Lobject* obj, fs::path& path, const fs::path& projectPath);
    static void load_imported_mesh(Mesh& lmesh);
};
}

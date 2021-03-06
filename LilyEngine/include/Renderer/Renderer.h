#pragma once

#include <filesystem>
#include <cstdio>
#include <iostream>
#include <vector>

#include "Renderer/Graphics_headers.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"

#include "Entity/Component.h"

#include "Renderer/stb_image.h"

namespace Lily {
namespace fs = std::filesystem;

class Renderer {
public:
	Renderer();
	~Renderer();

	static void Initialize();

	static void Begin(Camera& cam);

    static void update_light(glm::vec3 pos);
    static void update_viewPos(glm::vec3 pos);

	static void SetClearColor(glm::vec4 clearColor);
	static void Clear();
	static void DrawMesh(const Mesh& mesh, const glm::mat4& tran);

	static void End();
private:
	static GLint modelMLocation;
	static GLint projectionMLocation;
	static GLint viewMLocation;
    static GLint viewPosLocation;

	static Shader* m_shader;

	static unsigned int m_default;

	static std::string ErrorString(GLenum error);
};

}

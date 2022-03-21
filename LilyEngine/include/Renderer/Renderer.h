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

class Renderer {
public:
	Renderer();
	~Renderer();

	static void Initialize();

	static void Begin(Camera& cam);

	static void SetClearColor(glm::vec4 clearColor);
	static void Clear();
	static void DrawMesh(const Mesh& mesh, const glm::mat4& tran);

	static void End();
			
	void toggle_mouselock();

private:
	static GLint modelMLocation;
	static GLint projectionMLocation;
	static GLint viewMLocation;

	static Shader* m_shader;

	static unsigned int m_default;

	static std::string ErrorString(GLenum error);
};

}

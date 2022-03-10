#pragma once

#include <filesystem>
#include <cstdio>
#include <iostream>
#include <vector>

#include "Renderer/Graphics_headers.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Importer.h"
#include "Entity/Components.h"

namespace Lily {



	class Renderer {
	public:
		Renderer();
		~Renderer();

		static void Initialize(const char* vertexFilename, const char* fragmentFilename);

		static void Begin(Camera& cam);

		static void SetClearColor(glm::vec4 clearColor);
		static void Clear();
		static void DrawMesh(Mesh_component& mesh);

		static void End();
				
		void ToggleMouseLock();

	private:
		static GLint modelMLocation;
		static GLint projectionMLocation;
		static GLint viewMLocation;

		static Shader* m_shader;

		static std::string ErrorString(GLenum error);
	};

}

#pragma once

#include <vector>

#include "GraphicsHeaders.h"
#include <iostream>
#include <fstream>

namespace Lily {
	class Shader {
	public:
		Shader();
		~Shader();

		bool Initialize();

		void Bind();
		bool AddShaderObject(GLenum ShaderType, const char* filename);

		bool Finalize();
		GLint GetUniformLocation(const char* pUniformName);


		void SetUniformvec3(const char* name, glm::vec3 value);

	private:    
		GLuint m_shaderProg;
		std::vector<GLuint> m_shaderObjList;
	};
}

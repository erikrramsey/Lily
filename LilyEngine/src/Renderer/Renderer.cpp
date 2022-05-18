#include "Renderer/Renderer.h"

namespace Lily {

	GLint Renderer::modelMLocation;
	GLint Renderer::projectionMLocation;
	GLint Renderer::viewMLocation;
	Shader* Renderer::m_shader;
	unsigned int Renderer::m_default;

    Renderer::Renderer() {
        
    }
	Renderer::~Renderer() {

	}

	void Renderer::Initialize() {
		glewInit();

		// Set up the shaders
		m_shader = new Shader();
		m_shader->Initialize();

		// add 2 shader objects, cant add more after compilation
		m_shader->AddShaderObject(GL_VERTEX_SHADER, "./assets/default.vs");
		m_shader->AddShaderObject(GL_FRAGMENT_SHADER, "./assets/default.fs");

		m_shader->Finalize();

		// Locate recquired uniforms in shader
		projectionMLocation = m_shader->GetUniformLocation("projectionMatrix");
		viewMLocation = m_shader->GetUniformLocation("viewMatrix");
		modelMLocation = m_shader->GetUniformLocation("modelMatrix");

		//enable depth testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_DEBUG_OUTPUT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LESS);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// get any errors from OpenGL
		auto error = glGetError();
		if ( error != GL_NO_ERROR ) {
		  std::string val = ErrorString( error );
		  std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
		}

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
		width = 100;
		height = 100;
		nrComponents = 4;
		auto data = stbi_load("C:\\Dev\\Lily\\bin\\Debug\\assets\\default.png", &width, &height, &nrComponents, 0);

		GLenum format;
		switch (nrComponents) {
		case 1:
			format = GL_RED; break;
		case 2:
			format = GL_GREEN; break;
		case 3:
			format = GL_RGB; break;
		default:
		case 4:
			format = GL_RGBA; break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		m_default = textureID;
	}


	void Renderer::SetClearColor(glm::vec4 clearColor) {
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::Begin(Camera& cam) {
		m_shader->Bind();

		// Send in the projection and view to the shader
		glUniformMatrix4fv(projectionMLocation, 1, GL_FALSE, glm::value_ptr(cam.GetProjection())); 
		glUniformMatrix4fv(viewMLocation,		1, GL_FALSE, glm::value_ptr(cam.GetView()));


		// get any errors from OpenGL
		auto error = glGetError();
		if ( error != GL_NO_ERROR ) {
		  std::string val = ErrorString( error );
		  std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
		}
    }


void Renderer::DrawMesh(const Mesh& mesh, const glm::mat4& tran) {
	for (int i = 0; i < mesh.textures.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
	}
	m_shader->Bind();
	glUniformMatrix4fv(Renderer::modelMLocation, 1, GL_FALSE, glm::value_ptr(tran));

	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, m_default);
}

  std::string Renderer::ErrorString(GLenum error) {
    if(error == GL_INVALID_ENUM)
    {
      return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
    }

    else if(error == GL_INVALID_VALUE)
    {
      return "GL_INVALID_VALUE: A numeric argument is out of range.";
    }

    else if(error == GL_INVALID_OPERATION)
    {
      return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
    }

    else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
    {
      return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
    }

    else if(error == GL_OUT_OF_MEMORY)
    {
      return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
    }
    else
    {
      return "None";
    }
  }

}

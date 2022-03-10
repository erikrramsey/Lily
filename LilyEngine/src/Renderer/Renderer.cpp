#include "Renderer/Renderer.h"

namespace Lily {

	GLint Renderer::modelMLocation;
	GLint Renderer::projectionMLocation;
	GLint Renderer::viewMLocation;
	Shader* Renderer::m_shader;

    Renderer::Renderer() {
        
    }
	Renderer::~Renderer() {

	}

	void Renderer::Initialize(const char* vertexFilename, const char* fragmentFilename) {
		glewInit();

		// Set up the shaders
		m_shader = new Shader();
		m_shader->Initialize();

		// add 2 shader objects, cant add more after compilation
		m_shader->AddShaderObject(GL_VERTEX_SHADER, vertexFilename);
		m_shader->AddShaderObject(GL_FRAGMENT_SHADER, fragmentFilename);

		m_shader->Finalize();

		// Locate recquired uniforms in shader
		projectionMLocation = m_shader->GetUniformLocation("projectionMatrix");
		viewMLocation = m_shader->GetUniformLocation("viewMatrix");
		modelMLocation = m_shader->GetUniformLocation("modelMatrix");

		//enable depth testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glDepthFunc(GL_LESS);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Get any errors from OpenGL
		auto error = glGetError();
		if ( error != GL_NO_ERROR ) {
		  std::string val = ErrorString( error );
		  std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
		}
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


		// Get any errors from OpenGL
		auto error = glGetError();
		if ( error != GL_NO_ERROR ) {
		  std::string val = ErrorString( error );
		  std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
		}
    }


  void Renderer::DrawMesh(Mesh_component& mesh) {
	  if (mesh.mesh.textures.size() > 0) {
		  glBindTexture(GL_TEXTURE_2D, mesh.mesh.textures[0].id);
	  }
	  m_shader->Bind();
      glUniformMatrix4fv(Renderer::modelMLocation, 1, GL_FALSE, glm::value_ptr(mesh.basis));

      glBindVertexArray(mesh.mesh.VAO);
      glDrawElements(GL_TRIANGLES, mesh.mesh.indices.size(), GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
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

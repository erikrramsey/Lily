#pragma once

#include "Graphics_headers.h"

namespace Lily {


	struct framebufferInfo {
		glm::vec2 resolution;
	};


	class Framebuffer {
	public:
		Framebuffer(framebufferInfo info) : m_info(info) {}
		Framebuffer(int height, int width);

		~Framebuffer() = default;

		void Init();
		void Bind();
		void Unbind();

		void Resize(int height, int width);

		unsigned int GetTextureAttachment() { return m_fb_texture; }
		glm::vec2 GetResolution() { return m_info.resolution; }

	private:
		GLuint m_FBO;
		GLuint m_fb_texture;
		GLuint m_stencil_depth_RBO;
		framebufferInfo m_info;
	};

}
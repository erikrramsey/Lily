#include "Renderer/Framebuffer.h"

namespace Lily {
	Framebuffer::Framebuffer(int width, int height) {
		m_info.resolution.y = height;
		m_info.resolution.x = width;
		m_FBO = GL_INVALID_VALUE;
		m_fb_texture = GL_INVALID_VALUE;
		m_stencil_depth_RBO = GL_INVALID_VALUE;

		glGenFramebuffers(1, &m_FBO);
	}

	void Framebuffer::Init() {
		Bind();

		glGenTextures(1, &m_fb_texture);
		glBindTexture(GL_TEXTURE_2D, m_fb_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_info.resolution.x, m_info.resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fb_texture, 0);

		glGenRenderbuffers(1, &m_stencil_depth_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_stencil_depth_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_info.resolution.x, m_info.resolution.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil_depth_RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		// TEMP ??
		glViewport(0, 0, m_info.resolution.x, m_info.resolution.y);

		Unbind();
	}

	void Framebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void Framebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(int width, int height) {
		m_info.resolution.x = width;
		m_info.resolution.y = height;
		Init();
	}
}

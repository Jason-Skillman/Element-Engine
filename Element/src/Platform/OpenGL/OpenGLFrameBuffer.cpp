#include "pch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Element {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProperties& properties)
		: rendererID(0), properties(properties) {

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &rendererID);
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
	}

	void OpenGLFrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
		properties.width = width;
		properties.height = height;
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate() {
		if(rendererID) {
			glDeleteFramebuffers(1, &rendererID);
			glDeleteTextures(1, &colorAttachment);
			glDeleteTextures(1, &depthAttachment);
		}
		
		glCreateFramebuffers(1, &rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

		
		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, properties.width, properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);


		
		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, properties.width, properties.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);


		EL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete!");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

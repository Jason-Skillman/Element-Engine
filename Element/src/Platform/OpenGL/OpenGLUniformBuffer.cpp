#include "pch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Element {

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
		EL_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererID);
		glNamedBufferData(rendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		EL_PROFILE_FUNCTION();

		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
		EL_PROFILE_FUNCTION();

		glNamedBufferSubData(rendererID, offset, size, data);
	}
}

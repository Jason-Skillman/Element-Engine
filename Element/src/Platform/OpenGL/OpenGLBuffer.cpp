#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Element {

	//----- Vertex Buffer -----
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		: rendererId(0) {

		EL_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
		: rendererId(0) {

		EL_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		EL_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &rendererId);
	}
	
	void OpenGLVertexBuffer::Bind() const {
		EL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
	}
	
	void OpenGLVertexBuffer::Unbind() const {
		EL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	//----- Index Buffer -----
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: rendererId(0), count(count) {

		EL_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	
	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		EL_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &rendererId);
	}
	
	void OpenGLIndexBuffer::Bind() const {
		EL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
	}
	
	void OpenGLIndexBuffer::Unbind() const {
		EL_PROFILE_FUNCTION();
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

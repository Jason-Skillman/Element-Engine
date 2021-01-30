#include "hzpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {

	//----- Vertex Buffer -----
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
		: rendererId(0) {

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &rendererId);
	}
	
	void OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
	}
	
	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	//----- Index Buffer -----
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: rendererId(0), count(count) {

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	
	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &rendererId);
	}
	
	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
	}
	
	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

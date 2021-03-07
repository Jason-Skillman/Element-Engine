#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazel {

	//Todo: Move OpenGL content
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch(type) {
			case ShaderDataType::Float:
				return GL_FLOAT;
			case ShaderDataType::Float2:
				return GL_FLOAT;
			case ShaderDataType::Float3:
				return GL_FLOAT;
			case ShaderDataType::Float4:
				return GL_FLOAT;
			case ShaderDataType::Mat3:
				return GL_FLOAT;
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			case ShaderDataType::Int:
				return GL_INT;
			case ShaderDataType::Int2:
				return GL_INT;
			case ShaderDataType::Int3:
				return GL_INT;
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::Bool:
				return GL_BOOL;
			default:
				HZ_CORE_ASSERT(true, "Unknown ShaderDataType: {0}", type);
				return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: rendererID(0) {

		HZ_PROFILE_FUNCTION();
		
		glCreateVertexArrays(1, &rendererID);
	}
	
	OpenGLVertexArray::~OpenGLVertexArray() {
		HZ_PROFILE_FUNCTION();
		
		glDeleteVertexArrays(1, &rendererID);
	}
	
	void OpenGLVertexArray::Bind() const {
		HZ_PROFILE_FUNCTION();
		
		glBindVertexArray(rendererID);
	}
	
	void OpenGLVertexArray::Unbind() const {
		HZ_PROFILE_FUNCTION();
		
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		HZ_PROFILE_FUNCTION();
		
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!");
		
		glBindVertexArray(rendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		for(const auto& element : vertexBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE, vertexBuffer->GetLayout().GetStride(), reinterpret_cast<const void*>(element.offset));

			index++;
		}

		vertexBuffers.push_back(vertexBuffer);
	}
	
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		HZ_PROFILE_FUNCTION();
		
		glBindVertexArray(rendererID);
		indexBuffer->Bind();

		this->indexBuffer = indexBuffer;
	}
}

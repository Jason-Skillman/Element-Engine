#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Element {

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
				EL_CORE_FAIL("Unknown type: {0}", type);
				return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: rendererID(0) {

		EL_PROFILE_FUNCTION();
		
		glCreateVertexArrays(1, &rendererID);
	}
	
	OpenGLVertexArray::~OpenGLVertexArray() {
		EL_PROFILE_FUNCTION();
		
		glDeleteVertexArrays(1, &rendererID);
	}
	
	void OpenGLVertexArray::Bind() const {
		EL_PROFILE_FUNCTION();
		
		glBindVertexArray(rendererID);
	}
	
	void OpenGLVertexArray::Unbind() const {
		EL_PROFILE_FUNCTION();
		
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		EL_PROFILE_FUNCTION();
		
		EL_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!");
		
		glBindVertexArray(rendererID);
		vertexBuffer->Bind();

		const BufferLayout& layout = vertexBuffer->GetLayout();
		for(const auto& element : layout) {

			switch(element.type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.offset));
					vertexBufferIndex++;
					break;
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribIPointer(vertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
						layout.GetStride(), reinterpret_cast<const void*>(element.offset));
					vertexBufferIndex++;
					break;
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
					//Todo: Backwards compatable, should still work
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.offset));
					vertexBufferIndex++;
					break;
				default:
					EL_CORE_FAIL("Unknown type: {0}", element.type);
					break;
			}
		}

		vertexBuffers.push_back(vertexBuffer);
	}
	
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		EL_PROFILE_FUNCTION();
		
		glBindVertexArray(rendererID);
		indexBuffer->Bind();

		this->indexBuffer = indexBuffer;
	}
}

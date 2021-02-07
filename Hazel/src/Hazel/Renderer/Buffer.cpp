#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
			default:
				HZ_CORE_ASSERT(true, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return new OpenGLIndexBuffer(indices, size);
			default:
				HZ_CORE_ASSERT(true, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

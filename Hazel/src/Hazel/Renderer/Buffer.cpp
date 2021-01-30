#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch(Renderer::GetRendererApi()) {
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
			default:
				HZ_CORE_ASSERT(false, "No compatible renderer API selected: {0}", Renderer::GetRendererApi());
				return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch(Renderer::GetRendererApi()) {
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indices, size);
			default:
				HZ_CORE_ASSERT(false, "No compatible renderer API selected: {0}", Renderer::GetRendererApi());
				return nullptr;
		}
	}
}

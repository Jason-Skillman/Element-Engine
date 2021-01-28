#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch(Renderer::GetRendererApi()) {
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		HZ_CORE_ERROR("No renderer API selected. RendererAPI: {0}", Renderer::GetRendererApi());
		HZ_CORE_ASSERT(false, "No renderer API selected.");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch(Renderer::GetRendererApi()) {
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}
		HZ_CORE_ERROR("No renderer API selected. RendererAPI: {0}", Renderer::GetRendererApi());
		HZ_CORE_ASSERT(false, "No renderer API selected.");
		return nullptr;
	}
}

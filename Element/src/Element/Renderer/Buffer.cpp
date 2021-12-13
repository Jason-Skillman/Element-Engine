#include "pch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Element {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(size);
			default:
				EL_CORE_FAIL("No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
			default:
				EL_CORE_FAIL("No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, count);
			default:
				EL_CORE_FAIL("No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

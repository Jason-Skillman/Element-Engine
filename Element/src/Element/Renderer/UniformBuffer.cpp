#include "pch.h"
#include "UniformBuffer.h"

#include "Element/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Element {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLUniformBuffer>(size, binding);
			default:
				EL_CORE_ASSERT(false, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

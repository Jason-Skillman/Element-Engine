#include "pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Element {

	Ref<VertexArray> VertexArray::Create() {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLVertexArray>();
			default:
				HZ_CORE_ASSERT(true, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

#include "hzpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	VertexArray* VertexArray::Create() {
		switch(Renderer::GetRendererApi()) {
			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
			default:
				HZ_CORE_ASSERT(false, "No compatible renderer API selected: {0}", Renderer::GetRendererApi());
				return nullptr;
		}
	}
}

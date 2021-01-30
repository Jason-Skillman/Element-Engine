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
		}
		
		HZ_CORE_ERROR("No renderer API selected. RendererAPI: {0}", Renderer::GetRendererApi());
		HZ_CORE_ASSERT(false, "No renderer API selected.");
		return nullptr;
	}
}

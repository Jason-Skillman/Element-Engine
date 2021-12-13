#include "pch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Element {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLFrameBuffer>(specification);
			default:
				EL_CORE_FAIL("No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

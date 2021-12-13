#include "pch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Element {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
			default:
				EL_CORE_FAIL("No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
			default:
				EL_CORE_FAIL("No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

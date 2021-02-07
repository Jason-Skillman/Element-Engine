#include "hzpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLTexture2D>(path);
			default:
				HZ_CORE_ASSERT(true, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

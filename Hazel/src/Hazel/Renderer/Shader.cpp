#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Shader* Shader::Create(const std::string& filepath) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return new OpenGLShader(filepath);
			default:
				HZ_CORE_ASSERT(false, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
	
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return new OpenGLShader(vertexSrc, fragmentSrc);
			default:
				HZ_CORE_ASSERT(false, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
}

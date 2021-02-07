#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(filepath);
			default:
				HZ_CORE_ASSERT(true, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}
	
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch(Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
			default:
				HZ_CORE_ASSERT(true, "No compatible renderer API selected: {0}", Renderer::GetAPI());
				return nullptr;
		}
	}


	
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		HZ_CORE_ASSERT(Exists(name), "Shader \"{0}\" already exists in shader library!", name);
		shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		const std::string& name = shader->GetName();
		HZ_CORE_ASSERT(Exists(name), "Shader \"{0}\" already exists in shader library!", name);
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}
	
	Ref<Shader> ShaderLibrary::Get(const std::string& name) const {
		HZ_CORE_ASSERT(!Exists(name), "Shader \"{0}\" not found!", name);
		//return shaders[name];
		return shaders.at(name);
	}

	bool ShaderLibrary::Exists(const std::string& name) const {
		return shaders.find(name) != shaders.end();
	}
}

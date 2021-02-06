#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "Hazel/Renderer/Shader.h"

//Todo: Remove
typedef unsigned int GLenum;

namespace Hazel {
	class OpenGLShader : public Shader {
	private:
		uint32_t rendererId;
		mutable std::unordered_map<std::string, int> locationCache;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetUniformInt(const std::string& name, int value);
		
		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat2(const std::string& name, const glm::vec2& value);
		void SetUniformFloat3(const std::string& name, const glm::vec3& value);
		void SetUniformFloat4(const std::string& name, const glm::vec4& value);
		
		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		
		int OpenGLShader::GetUniformLocation(const std::string& name) const;
	};
}

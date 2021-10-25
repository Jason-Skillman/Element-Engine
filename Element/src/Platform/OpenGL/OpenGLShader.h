#pragma once

#include <glm/glm.hpp>

#include "Element/Renderer/Shader.h"

//Todo: Remove
typedef unsigned int GLenum;

namespace Element {
	class OpenGLShader : public Shader {
	private:
		uint32_t rendererId;
		std::string name;
		mutable std::unordered_map<std::string, int> locationCache;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformInt(const std::string& name, int value) override;
		virtual void SetUniformIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetUniformFloat(const std::string& name, float value) override;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& vector) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vector) override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vector) override;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;

		virtual uint32_t GetRendererId() const override {
			return rendererId;
		}

		virtual const std::string& GetName() const override {
			return name;
		}

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vector);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		
		int OpenGLShader::GetUniformLocation(const std::string& name) const;
	};
}

#pragma once

#include <glm/fwd.hpp>

#include "Element/Core/Core.h"

namespace Element {

	//Abstract
	class Shader {
	public:
		virtual ~Shader() = default;

	public:
		static Ref<Shader> Create(const std::string& filepath);
		[[deprecated("Use Create(std::string) instead.")]]
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const std::string& name, int value) = 0;
		virtual void SetUniformIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetUniformFloat(const std::string& name, float value) = 0;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& vector) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vector) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vector) = 0;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

		virtual uint32_t GetRendererId() const = 0;
		virtual const std::string& GetName() const = 0;
	};

	class ShaderLibrary {
	private:
		std::unordered_map<std::string, Ref<Shader>> shaders;

	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name) const;

		bool Exists(const std::string& name) const;
	};
}

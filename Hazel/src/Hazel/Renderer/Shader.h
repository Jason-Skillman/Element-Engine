#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {
	/// <summary>
	/// Abstract Shader class
	/// </summary>
	class Shader {
	public:
		virtual ~Shader() = default;

	public:
		static Ref<Shader> Create(const std::string& filepath);
		[[deprecated("Use Create(std::string) instead.")]]
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		inline virtual const std::string& GetName() const = 0;
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

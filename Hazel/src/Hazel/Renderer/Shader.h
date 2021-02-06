#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Hazel {
	class Shader {
	public:
		virtual ~Shader() = default;

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& filepath);
		[[deprecated("Use Create(std::string) instead.")]]
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}

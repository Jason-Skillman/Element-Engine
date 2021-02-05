#pragma once

#include <glm/glm.hpp>

namespace Hazel {
	class Shader {
	private:
		uint32_t rendererId;
		
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

	public:
		void Bind() const;
		void Unbind() const;

		void SetUniformMat4fv(const std::string& name, const glm::mat4& matrix);
		void SetUniformFloat4(const std::string& name, const glm::vec4& values);
	};
}

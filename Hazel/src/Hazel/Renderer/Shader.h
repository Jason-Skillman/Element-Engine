#pragma once

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
	};
}

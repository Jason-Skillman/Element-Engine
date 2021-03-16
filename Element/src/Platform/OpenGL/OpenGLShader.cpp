#include "pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Element {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if(type == "vertex") return GL_VERTEX_SHADER;
		if(type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(true, "Unknown shader type: {0}", type);
		return -1;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		HZ_PROFILE_FUNCTION();
		
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//Extract name from file path
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: rendererID(-1), name(name) {

		HZ_PROFILE_FUNCTION();
		
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		HZ_PROFILE_FUNCTION();
		
		glDeleteProgram(rendererID);
	}

	void OpenGLShader::Bind() const {
		HZ_PROFILE_FUNCTION();
		
		glUseProgram(rendererID);
	}

	void OpenGLShader::Unbind() const {
		HZ_PROFILE_FUNCTION();
		
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value) {
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, uint32_t count) {
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value) {
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& vector) {
		UploadUniformFloat2(name, vector);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& vector) {
		UploadUniformFloat3(name, vector);
	}
	
	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& vector) {
		UploadUniformFloat4(name, vector);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) {
		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {
		UploadUniformMat4(name, matrix);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
		glUniform1iv(GetUniformLocation(name), count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector) {
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector) {
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector) {
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string Element::OpenGLShader::ReadFile(const std::string& filepath) {
		HZ_PROFILE_FUNCTION();
		
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if(in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		} else {
			LOG_CORE_ERROR("Could not open file \"{0}\"", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> Element::OpenGLShader::PreProcess(const std::string& source) {
		HZ_PROFILE_FUNCTION();
		
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		//Loop through every type token found
		while(pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol == std::string::npos, "Syntax error!");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(type != "vertex" && type != "fragment" && type != "pixel", "Invalid shader type specified: {0}", type);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() -1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		HZ_PROFILE_FUNCTION();
		
		// Get a program object.
		unsigned int program = glCreateProgram();

		//Keep track of the rendererIDs
		HZ_CORE_ASSERT(shaderSources.size() > 2, "Only 2 shaders are supported for now.")
		std::array<GLenum, 2> glShaderIds;

		int index = 0;
		for(auto& pair : shaderSources) {
			GLenum type = pair.first;
			const std::string& source = pair.second;

			
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				HZ_CORE_ASSERT(true, "{0} shader failed to compile: {1}", type, infoLog.data());
				return;;
			}

			glAttachShader(program, shader);
			glShaderIds[index] = shader;

			index++;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if(isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for(auto id : glShaderIds)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			HZ_CORE_ASSERT(true, "Shader failed to link: {0}", infoLog.data());
			return;
		}

		// Always detach shaders after a successful link.
		for(auto id : glShaderIds)
			glDetachShader(program, id);

		rendererID = program;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const {
		if(locationCache.find(name) != locationCache.end())
			return locationCache[name];

		int location = glGetUniformLocation(rendererID, name.c_str());
		//HZ_CORE_ASSERT(location == -1, "Uniform {0} could not be found!", name.c_str());

		locationCache[name] = location;

		return location;
	}
}

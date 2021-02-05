#include "hzpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
		// Read our shaders into the appropriate buffers
		std::string vertexSource = vertexSrc;
		std::string fragmentSource = fragmentSrc;

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader compilation error!")

				// In this simple program, we'll just leave
				return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader compilation error!")

				// In this simple program, we'll just leave
				return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		rendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(rendererId, vertexShader);
		glAttachShader(rendererId, fragmentShader);

		// Link our program
		glLinkProgram(rendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererId, GL_LINK_STATUS, (int*)&isLinked);
		if(isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(rendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(rendererId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader link error!")

				// In this simple program, we'll just leave
				return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(rendererId, vertexShader);
		glDetachShader(rendererId, fragmentShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(rendererId);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const {
		if(locationCache.find(name) != locationCache.end())
			return locationCache[name];

		int location = glGetUniformLocation(rendererId, name.c_str());
		HZ_CORE_ASSERT(location != -1, "Uniform {0} could not be found!", name.c_str());

		locationCache[name] = location;

		return location;
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformInt(const std::string& name, int value) {
		glUniform1i(GetUniformLocation(name), value);
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformFloat(const std::string& name, float value) {
		glUniform1f(GetUniformLocation(name), value);
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value) {
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value) {
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value) {
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	/// <summary>
	/// <para>Shader must be bound before setting uniform.</para>
	/// </summary>
	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

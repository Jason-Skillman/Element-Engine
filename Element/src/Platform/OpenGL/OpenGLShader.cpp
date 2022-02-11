#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Element/Core/Timer.h"

namespace Element {

	namespace Utils {

		static GLenum ShaderTypeFromString(const std::string& type) {
			if(type == "vertex")
				return GL_VERTEX_SHADER;
			if(type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;
			EL_CORE_FAIL("Unknown type: {0}, ", type);
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch(stage) {
				case GL_VERTEX_SHADER:
					return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER:
					return shaderc_glsl_fragment_shader;
				default:
					EL_CORE_FAIL("Unknown type: {0}", stage);
					return (shaderc_shader_kind)0;
			}
		}

		static const char* GLShaderStageToString(GLenum stage) {
			switch(stage) {
				case GL_VERTEX_SHADER:
					return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER:
					return "GL_FRAGMENT_SHADER";
				default:
					EL_CORE_FAIL("Unknown type: {0}", stage);
					return nullptr;
			}
		}

		static const char* GetCacheDirectory() {
			//Todo: Make sure the assets directory is valid
			return "Cache/Shaders/OpenGL";
		}

		static void CreateCacheDirectoryIfNeeded() {
			std::string cacheDirectory = GetCacheDirectory();
			if(!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
			switch(stage) {
				case GL_VERTEX_SHADER:
					return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:
					return ".cached_opengl.frag";
				default:
					EL_CORE_FAIL("Unknown type: {0}", stage);
					return "";
			}
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
			switch(stage) {
				case GL_VERTEX_SHADER:
					return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:
					return ".cached_vulkan.frag";
				default:
					EL_CORE_FAIL("Unknown type: {0}", stage);
					return "";
			}
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: filePath(filepath) {

		EL_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			EL_CORE_LOG_INFO("Shaders compiled in {0} ms", timer.ElapsedMillis());
		}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: name(name) {

		EL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader() {
		EL_PROFILE_FUNCTION();

		glDeleteProgram(rendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		EL_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if(in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if(size != -1) {
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			} else
				EL_CORE_FAIL("Could not read from file '{0}'", filepath);
		} else
			EL_CORE_FAIL("Could not open file '{0}'", filepath);

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		EL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while(pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			EL_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			EL_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			EL_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources) {
		EL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if(optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = vulkanSPIRV;
		shaderData.clear();
		for(auto&& [stage, source] : shaderSources) {
			std::filesystem::path shaderFilePath = filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if(in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			} else {
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), filePath.c_str(), options);
				if(module.GetCompilationStatus() != shaderc_compilation_status_success) {
					EL_CORE_LOG_ERROR(module.GetErrorMessage());
					EL_CORE_FAIL("Shader could not be compiled! The shader probably has a syntax error.");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if(out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		EL_CORE_LOG_INFO("");
		EL_CORE_LOG_INFO("SPIR-V Shader Compiler");
		for(auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries() {
		EL_PROFILE_FUNCTION();

		auto& shaderData = openGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if(optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		openGLSourceCode.clear();
		for(auto&& [stage, spirv] : vulkanSPIRV) {
			std::filesystem::path shaderFilePath = filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if(in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			} else {
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				openGLSourceCode[stage] = glslCompiler.compile();
				auto& source = openGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), filePath.c_str());
				if(module.GetCompilationStatus() != shaderc_compilation_status_success) {
					EL_CORE_FAIL("Error in OpenGLShader::CompileOrGetOpenGLBinaries(): {0}", module.GetErrorMessage());
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if(out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram() {
		EL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for(auto&& [stage, spirv] : openGLSPIRV) {
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if(isLinked == GL_FALSE) {
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			EL_CORE_LOG_ERROR("Shader linking failed ({0}):\n{1}", filePath, infoLog.data());

			glDeleteProgram(program);

			for(auto id : shaderIDs)
				glDeleteShader(id);
		}

		for(auto id : shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		rendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData) {
		EL_PROFILE_FUNCTION();

		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		EL_CORE_LOG_INFO("  Compiling shader \"{0}\" at \"{1}\"", Utils::GLShaderStageToString(stage), filePath);
		EL_CORE_LOG_INFO("    {0} uniform buffers", resources.uniform_buffers.size());
		EL_CORE_LOG_INFO("    {0} resources", resources.sampled_images.size());

		if(!resources.uniform_buffers.empty())
			EL_CORE_LOG_INFO("  Uniform buffers:");
		for(const auto& resource : resources.uniform_buffers) {
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			EL_CORE_LOG_INFO("    {0}", resource.name);
			EL_CORE_LOG_INFO("      Size = {0}", bufferSize);
			EL_CORE_LOG_INFO("      Binding = {0}", binding);
			EL_CORE_LOG_INFO("      Members = {0}", memberCount);
		}
	}

	void OpenGLShader::Bind() const {
		EL_PROFILE_FUNCTION();

		glUseProgram(rendererID);
	}

	void OpenGLShader::Unbind() const {
		EL_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value) {
		EL_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, uint32_t count) {
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value) {
		EL_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value) {
		EL_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value) {
		EL_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value) {
		EL_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value) {
		EL_PROFILE_FUNCTION();

		UploadUniformMat3(name, value);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
		EL_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

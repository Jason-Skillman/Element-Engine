#include "pch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Element/Core/Core.h"

namespace Element {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: width(width), height(height) {
		
		EL_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, m_InternalFormat, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: path(path) {

		EL_PROFILE_FUNCTION();

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			EL_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D - stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		EL_CORE_ASSERT(data, "Failed to load image: {0}", path);

		GLenum internalFormat = 0, dataFormat = 0;
		if(channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if(channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;
		//EL_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported! path: {0}, channels: {1}", path, channels);
		
		this->width = width;
		this->height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, m_InternalFormat, this->width, this->height);
		
		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererID, 0, 0, 0, this->width, this->height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	
	OpenGLTexture2D::~OpenGLTexture2D() {
		EL_PROFILE_FUNCTION();
		
		glDeleteTextures(1, &rendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		EL_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		EL_CORE_ASSERT(size == width * height * bpp, "Data must be the entire texture!");
		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	
	void OpenGLTexture2D::Bind(uint32_t slot) const {
		EL_PROFILE_FUNCTION();
		
		glBindTextureUnit(slot, rendererID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const {
		EL_PROFILE_FUNCTION();
		
		glBindTextureUnit(slot, 0);
	}
}

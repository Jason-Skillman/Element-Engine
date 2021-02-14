#include "pch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Hazel/Core/Core.h"

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: path(path) {

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(!data, "Failed to load image: {0}", path);

		GLenum internalFormat = 0, dataFormat = 0;
		if(channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if(channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		//HZ_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported! path: {0}, channels: {1}", path, channels);
		
		this->width = width;
		this->height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, internalFormat, this->width, this->height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererId, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	
	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &rendererId);
	}
	
	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, rendererId);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const {
		glBindTextureUnit(slot, 0);
	}
}

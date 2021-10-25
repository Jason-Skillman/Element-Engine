#pragma once

#include <glad/glad.h>

#include "Element/Renderer/Texture.h"

namespace Element {
	class OpenGLTexture2D : public Texture2D {
	private:
		uint32_t rendererId;
		uint32_t width, height;
		std::string path;
		GLenum m_InternalFormat, m_DataFormat;
		
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();
		
	public:
		virtual uint32_t GetWidth() const override {
			return width;
		}

		virtual uint32_t GetHeight() const override {
			return height;
		}

		virtual uint32_t GetRendererId() const override {
			return rendererId;
		}

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind(uint32_t slot) const override;

	public:
		virtual bool operator==(const Texture& other) const override {
			return rendererId == ((OpenGLTexture2D&)other).rendererId;
		}

	};
}

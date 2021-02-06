#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel {
	class OpenGLTexture2D : public Texture2D {
	private:
		uint32_t rendererId;
		uint32_t width, height;
		std::string path;
		
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();
		
	public:
		inline virtual uint32_t GetWidth() const override {
			return width;
		}
		
		inline virtual uint32_t GetHeight() const override {
			return height;
		}
		
		virtual void Bind(uint32_t slot = 0) const override;
	};
}

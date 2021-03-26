#pragma once

#include "Element/Renderer/FrameBuffer.h"

namespace Element {
	class OpenGLFrameBuffer : public FrameBuffer {
	private:
		uint32_t rendererID;
		uint32_t colorAttachment;
		uint32_t depthAttachment;
		FrameBufferProperties properties;

	public:
		OpenGLFrameBuffer(const FrameBufferProperties& properties);
		virtual ~OpenGLFrameBuffer();

	public:
		inline virtual const FrameBufferProperties& GetProperties() const override {
			return properties;
		}

		inline virtual const uint32_t GetRendererID() const override {
			return rendererID;
		}

		inline virtual uint32_t GetColorAttachmentRendererID() override {
			return colorAttachment;
		}
		
		virtual void Bind() override;
		virtual void Unbind() override;

		void Invalidate();
		
	};
}

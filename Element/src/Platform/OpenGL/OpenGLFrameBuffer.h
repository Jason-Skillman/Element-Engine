#pragma once

#include "Element/Renderer/FrameBuffer.h"

namespace Element {
	class OpenGLFrameBuffer : public FrameBuffer {
	private:
		uint32_t rendererId;
		uint32_t colorAttachment;
		uint32_t depthAttachment;
		FrameBufferProperties properties;

	public:
		OpenGLFrameBuffer(const FrameBufferProperties& properties);
		virtual ~OpenGLFrameBuffer();

	public:
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FrameBufferProperties& GetProperties() const override {
			return properties;
		}

		virtual uint32_t GetRendererId() const override {
			return rendererId;
		}

		virtual uint32_t GetColorAttachmentRendererId() const override {
			return colorAttachment;
		}

		void Invalidate();
	};
}

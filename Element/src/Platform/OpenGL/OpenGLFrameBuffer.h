#pragma once

#include "Element/Renderer/FrameBuffer.h"

namespace Element {

	class OpenGLFrameBuffer : public FrameBuffer {
	private:
		uint32_t rendererID = 0;
		FrameBufferSpecification specification;

		std::vector<FrameBufferTextureSpecification> colorAttachmentSpecifications;
		FrameBufferTextureSpecification depthAttachmentSpecification;

		std::vector<uint32_t> colorAttachmentIDs;
		uint32_t depthAttachmentID = 0;

	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();

	public:
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FrameBufferSpecification& GetSpecification() const override {
			return specification;
		}

		virtual uint32_t GetRendererID() const override {
			return rendererID;
		}

		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override {
			EL_CORE_ASSERT(index < colorAttachmentIDs.size(), "Color attachment ID out of range!")
			return colorAttachmentIDs[index];
		}

		void Invalidate();
	};
}

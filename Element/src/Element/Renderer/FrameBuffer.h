#pragma once

#include "Element/Core/Core.h"

namespace Element {

	struct FrameBufferProperties {
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget;
	};
	
	class FrameBuffer {
	public:
		static Ref<FrameBuffer> Create(const FrameBufferProperties& properties);

		virtual const FrameBufferProperties& GetProperties() const = 0;
		virtual const uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}

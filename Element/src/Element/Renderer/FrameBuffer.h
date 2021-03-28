#pragma once

#include <glm/vec2.hpp>

#include "Element/Core/Core.h"

namespace Element {

	struct FrameBufferProperties {
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget;
	};

	//Abstract
	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		
	public:
		static Ref<FrameBuffer> Create(const FrameBufferProperties& properties);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FrameBufferProperties& GetProperties() const = 0;
		virtual const uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() = 0;
	};
}

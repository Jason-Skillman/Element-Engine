#pragma once

#include <glm/vec2.hpp>

#include "Element/Core/Core.h"

namespace Element {

	enum class FrameBufferTextureFormat {
		None = 0,

		//Single channel
		RED_INTEGER,		//Stored as an int instead of 0-1

		//Color
		RGBA8,

		//Depth
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8,
	};

	/// <summary>
	/// A single texture specification using FrameBufferTextureFormat.
	/// </summary>
	struct FrameBufferTextureSpecification {
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: textureFormat(format) {}

		FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::None;
		//Todo: Filtering/wrap
	};

	/// <summary>
	/// Collection of texture specifications.
	/// </summary>
	struct FrameBufferAttachmentSpecification {
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FrameBufferTextureSpecification> attachments;
	};

	struct FrameBufferSpecification {
		uint32_t width, height;
		FrameBufferAttachmentSpecification attachments;
		uint32_t samples = 1;

		bool swapChainTarget;
	};

	//Abstract
	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0;
	};
}

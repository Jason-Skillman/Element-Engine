#pragma once

#include <memory.h>

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {
	class VertexArray {
	public:
		virtual ~VertexArray() = default;

	public:
		static VertexArray* Create();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
	};
}

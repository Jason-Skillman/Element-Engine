#pragma once

#include "Element/Core/Core.h"
#include "Element/Renderer/Buffer.h"

namespace Element {

	class VertexArray {
	public:
		virtual ~VertexArray() = default;

	public:
		static Ref<VertexArray> Create();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
	};
}

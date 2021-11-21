#pragma once
#include "Element/Renderer/VertexArray.h"

namespace Element {

	class OpenGLVertexArray : public VertexArray {
	private:
		uint32_t rendererID;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
		uint32_t vertexBufferIndex = 0;
		
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual uint32_t GetRendererID() const override {
			return rendererID;
		}

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
			return vertexBuffers;
		}

		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override {
			return indexBuffer;
		}
	};
}

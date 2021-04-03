#pragma once
#include "Element/Renderer/VertexArray.h"

namespace Element {
	class OpenGLVertexArray : public VertexArray {
	private:
		uint32_t rendererId;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
		
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		inline virtual uint32_t GetRendererId() const override {
			return rendererId;
		}
		
		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
			return vertexBuffers;
		}
		
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override {
			return indexBuffer;
		}
	};
}

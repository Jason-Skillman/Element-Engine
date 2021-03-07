#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
	class OpenGLVertexArray : public VertexArray {
	private:
		uint32_t rendererID;
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
		
		virtual inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
			return vertexBuffers;
		}
		
		virtual inline const Ref<IndexBuffer>& GetIndexBuffer() const override {
			return indexBuffer;
		}
	};
}

#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
	class OpenGLVertexArray : public VertexArray {
	private:
		uint32_t rendererId;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
		
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		
		virtual inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
			return vertexBuffers;
		}
		
		virtual inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {
			return indexBuffer;
		}
	};
}

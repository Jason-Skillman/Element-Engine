#pragma once
#include "Element/Renderer/Buffer.h"

namespace Element {

	class OpenGLVertexBuffer : public VertexBuffer {
	private:
		uint32_t rendererID;
		BufferLayout layout;
		
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual uint32_t GetRendererID() const override {
			return rendererID;
		}

		virtual void SetLayout(const BufferLayout& layout) override {
			this->layout = layout;
		}

		virtual const BufferLayout& GetLayout() const override {
			return layout;
		}
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	private:
		uint32_t rendererID;
		uint32_t count;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetRendererID() const override {
			return rendererID;
		}

		virtual uint32_t GetCount() const override {
			return count;
		}
	};
}

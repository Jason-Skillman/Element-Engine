#pragma once
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {
	class OpenGLVertexBuffer : public VertexBuffer {
	private:
		uint32_t rendererId;
		
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	private:
		uint32_t rendererId;
		uint32_t count;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		inline virtual uint32_t GetCount() const override {
			return count;
		}
	};
}

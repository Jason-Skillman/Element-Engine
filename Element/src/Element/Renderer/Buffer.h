#pragma once
#include "ShaderDataType.h"

namespace Element {

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint64_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {}

		inline uint32_t GetComponentCount() const {
			return ShaderDataTypeCount(type);
		}
	};

	class BufferLayout {
	private:
		std::vector<BufferElement> elements;
		uint32_t stride;

	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: elements(elements), stride(0) {

			CalculateOffsetAndStride();
		}

	public:
		inline const std::vector<BufferElement>& GetElements() const {
			return elements;
		}

		inline uint32_t GetStride() const {
			return stride;
		}

		inline std::vector<BufferElement>::iterator begin() {
			return elements.begin();
		}

		inline std::vector<BufferElement>::const_iterator begin() const {
			return elements.begin();
		}

		inline std::vector<BufferElement>::iterator end() {
			return elements.end();
		}

		inline std::vector<BufferElement>::const_iterator end() const {
			return elements.end();
		}

	private:
		void CalculateOffsetAndStride() {
			uint32_t offset = 0;
			stride = 0;
			for(auto& element : elements) {
				element.offset = offset;
				offset += element.size;
				stride += element.size;
			}
		}
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

	public:
		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
	};

	//Engine only supports 32 bit index buffers
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

	public:
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}

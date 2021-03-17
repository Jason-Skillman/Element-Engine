#include "pch.h"
#include "Renderer2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Buffer.h"
#include "RenderCommand.h"

namespace Element {

	Renderer2D::RendererData Renderer2D::data;

	void Renderer2D::Init() {
		PROFILE_FUNCTION();
		
		//Data
		const BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float, "a_TextureIndex" },
			{ ShaderDataType::Float, "a_Tiling" }
		};

		//Objects
		data.quadVertexArray = VertexArray::Create();
		data.standardShader = Shader::Create("assets/shaders/glsl/Standard.shader");

		//Setup vertex buffer
		data.quadVertexBuffer = VertexBuffer::Create(data.maxVertices * sizeof(QuadVertex));
		data.quadVertexBuffer->SetLayout(layout);
		data.quadVertexArray->AddVertexBuffer(data.quadVertexBuffer);

		data.quadVertexBufferBase = new QuadVertex[data.maxVertices];

		//Setup index buffer
		uint32_t* quadIndices = new uint32_t[data.maxIndices];

		uint32_t offset = 0;
		for(uint32_t i = 0; i < data.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, data.maxIndices);
		data.quadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] quadIndices;

		

		//Create textures
		data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		
		int32_t samplers[data.maxTextureSlots];
		for(uint32_t i = 0; i < data.maxTextureSlots; i++) {
			samplers[i] = i;
		}
		data.standardShader->Bind();
		data.standardShader->SetUniformIntArray("u_Textures", samplers, data.maxTextureSlots);


		data.textureSlots[0] = data.whiteTexture;

		data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}
	
	void Renderer2D::Shutdown() {
		PROFILE_FUNCTION();
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PROFILE_FUNCTION();

		ResetStats();
		
		data.standardShader->Bind();
		data.standardShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;

		data.textureSlotIndex = 1;
	}
	
	void Renderer2D::EndScene() {
		PROFILE_FUNCTION();

		uint32_t dataSize = reinterpret_cast<uint8_t*>(data.quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.quadVertexBufferBase);
		data.quadVertexBuffer->SetData(data.quadVertexBufferBase, dataSize);
		
		Flush();
	}

	void Renderer2D::Flush() {
		PROFILE_FUNCTION();

		//Bind all textures to their texture slot
		for(uint32_t i = 0; i < data.textureSlotIndex; i++) {
			data.textureSlots[i]->Bind(i);
		}
		
		RenderCommand::DrawIndexed(data.quadVertexArray, data.quadIndexCount);
		data.stats.drawCalls++;
	}

	void Renderer2D::DrawQuad(const DrawProporties& properties) {
		PROFILE_FUNCTION();

		if(data.quadIndexCount >= data.maxIndices)
			FlushAndReset();

		//Texture batching
		float textureIndex = 0.0f;
		if(properties.texture != nullptr) {
			for(uint32_t i = 1; i < data.textureSlotIndex; i++) {
				//If the textures are equal
				if(*data.textureSlots[i].get() == *properties.texture.get()) {
					textureIndex = static_cast<float>(i);
					break;
				}
			}
			if(textureIndex == 0.0f) {
				textureIndex = static_cast<float>(data.textureSlotIndex);
				data.textureSlots[data.textureSlotIndex] = properties.texture;
				data.textureSlotIndex++;
			}
		}

		//Create the transform matrix
		glm::mat4 transform;
		if(properties.rotation == 0) {
			transform =
				glm::translate(glm::mat4(1.0f), properties.position) *
				glm::scale(glm::mat4(1.0f), { properties.scale.x, properties.scale.y, 1.0f });
		} else {
			transform =
				glm::translate(glm::mat4(1.0f), properties.position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(properties.rotation), { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), { properties.scale.x, properties.scale.y, 1.0f });
		}
		
		//Setup the vertex buffer
		data.quadVertexBufferPtr->position = transform * data.quadVertexPositions[0];
		data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->textureIndex = textureIndex;
		data.quadVertexBufferPtr->tiling = properties.tiling;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->position = transform * data.quadVertexPositions[1];
		data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->textureIndex = textureIndex;
		data.quadVertexBufferPtr->tiling = properties.tiling;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->position = transform * data.quadVertexPositions[2];
		data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->textureIndex = textureIndex;
		data.quadVertexBufferPtr->tiling = properties.tiling;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->position = transform * data.quadVertexPositions[3];
		data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->textureIndex = textureIndex;
		data.quadVertexBufferPtr->tiling = properties.tiling;
		data.quadVertexBufferPtr++;

		data.quadIndexCount += 6;

		data.stats.quadCount++;
	}

	void Renderer2D::ResetStats() {
		memset(&data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return data.stats;
	}

	void Renderer2D::FlushAndReset() {
		EndScene();

		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;

		data.textureSlotIndex = 1;
	}

}

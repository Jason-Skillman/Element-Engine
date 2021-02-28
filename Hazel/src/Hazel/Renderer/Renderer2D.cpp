#include "pch.h"
#include "Renderer2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Buffer.h"
#include "RenderCommand.h"

namespace Hazel {

	Renderer2D::RendererData Renderer2D::data;

	void Renderer2D::Init() {
		HZ_PROFILE_FUNCTION();
		
		//Data
		const BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		//Objects
		data.quadVertexArray = VertexArray::Create();
		data.textureShader = Shader::Create("assets/shaders/glsl/Texture.shader");

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

		//Set default uniforms
		data.textureShader->Bind();
		data.textureShader->SetUniformInt("u_Texture", 0);
	}
	
	void Renderer2D::Shutdown() {
		HZ_PROFILE_FUNCTION();
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		HZ_PROFILE_FUNCTION();
		
		data.textureShader->Bind();
		data.textureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;
	}
	
	void Renderer2D::EndScene() {
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = reinterpret_cast<uint8_t*>(data.quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.quadVertexBufferBase);
		data.quadVertexBuffer->SetData(data.quadVertexBufferBase, dataSize);
		
		Flush();
	}

	void Renderer2D::Flush() {
		HZ_PROFILE_FUNCTION();

		RenderCommand::DrawIndexed(data.quadVertexArray, data.quadIndexCount);
	}

	void Renderer2D::DrawQuad(const DrawProporties& properties) {
		HZ_PROFILE_FUNCTION();

		data.quadVertexBufferPtr->position = properties.position;
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f, };
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->position = { properties.position.x + properties.scale.x, properties.position.y, 0.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f, };
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->position = { properties.position.x + properties.scale.x, properties.position.y + properties.scale.y, 0.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f, };
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->position = { properties.position.x, properties.position.y + properties.scale.y, 0.0f };
		data.quadVertexBufferPtr->color = properties.color;
		data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f, };
		data.quadVertexBufferPtr++;

		data.quadIndexCount += 6;
		
		//Set uniforms
		data.textureShader->Bind();
		data.textureShader->SetUniformFloat4("u_Color", properties.color);
		data.textureShader->SetUniformFloat("u_TilingFactor", properties.tilingFactor);

		glm::mat4 transform;
		if(properties.rotation == 0) {
			transform =
				glm::translate(glm::mat4(1.0f), properties.position) *
				glm::scale(glm::mat4(1.0f), { properties.scale.x, properties.scale.y, 1.0f });
		} else {
			transform =
				glm::translate(glm::mat4(1.0f), properties.position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(properties.rotation), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), { properties.scale.x, properties.scale.y, 1.0f });
		}
		data.textureShader->SetUniformMat4("u_Transform", transform);

		//Bind objects
		//data.vertexArray->Bind();

		/*if(properties.texture == nullptr) {
			data.whiteTexture->Bind();
		} else {
			properties.texture->Bind();
		}*/

		//Render
		//RenderCommand::DrawIndexed(data.vertexArray);
	}

}

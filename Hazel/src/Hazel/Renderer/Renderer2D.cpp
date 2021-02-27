#include "pch.h"
#include "Renderer2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"

namespace Hazel {

	struct Renderer2DStorage {
		Ref<VertexArray> vertexArray;
		Ref<Shader> textureShader, colorShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage storage;

	void Renderer2D::Init() {
		HZ_PROFILE_FUNCTION();
		
		//Data
		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.8f, 0.0f, 1.0f, 1.0f
		};

		unsigned int indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		const unsigned int indicesSize = sizeof(indices) / sizeof(unsigned int);

		const BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		//Objects
		//storage = new Renderer2DStorage();
		storage.vertexArray = VertexArray::Create();
		storage.textureShader = Shader::Create("assets/shaders/glsl/Texture.shader");
		storage.colorShader = Shader::Create("assets/shaders/glsl/Color.shader");

		//Setup vertex buffer
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		storage.vertexArray->AddVertexBuffer(vertexBuffer);

		//Setup index buffer
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, indicesSize));
		storage.vertexArray->SetIndexBuffer(indexBuffer);

		//Set default uniforms
		storage.textureShader->SetUniformInt("u_Texture", 1);
		//storage->textureShader->SetUniformInt("u_Texture", 0);


		storage.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		storage.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}
	
	void Renderer2D::Shutdown() {
		HZ_PROFILE_FUNCTION();
		
		//delete storage;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		HZ_PROFILE_FUNCTION();
		
		storage.textureShader->Bind();
		storage.textureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	
	void Renderer2D::EndScene() {
		HZ_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, rotation, scale, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color) {
		storage.textureShader->Bind();
		storage.textureShader->SetUniformFloat4("u_Color", color);

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		storage.textureShader->SetUniformMat4("u_Transform", transform);

		
		storage.vertexArray->Bind();
		storage.whiteTexture->Bind();
		RenderCommand::DrawIndexed(storage.vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture, float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, rotation, scale, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture, float tilingFactor) {
		storage.textureShader->Bind();
		storage.textureShader->SetUniformFloat4("u_Color", glm::vec4(1.0f));
		storage.textureShader->SetUniformFloat("u_TilingFactor", tilingFactor);

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		storage.textureShader->SetUniformMat4("u_Transform", transform);

		texture->Bind();

		storage.vertexArray->Bind();
		RenderCommand::DrawIndexed(storage.vertexArray);
	}
}

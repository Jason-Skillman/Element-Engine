#include "pch.h"
#include "Renderer2D.h"

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"

namespace Hazel {

	struct Renderer2DStorage {
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};

	static Renderer2DStorage* storage;

	void Renderer2D::Init() {
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
		storage = new Renderer2DStorage();
		storage->vertexArray = VertexArray::Create();
		storage->shader = Shader::Create("assets/shaders/glsl/UniColor.shader");

		//Setup vertex buffer
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		storage->vertexArray->AddVertexBuffer(vertexBuffer);

		//Setup index buffer
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, indicesSize));
		storage->vertexArray->SetIndexBuffer(indexBuffer);
	}
	
	void Renderer2D::Shutdown() {
		delete storage;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		storage->shader->Bind();
		storage->shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		storage->shader->SetUniformMat4("u_Transform", glm::mat4(1.0f));
	}
	
	void Renderer2D::EndScene() {}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		storage->shader->Bind();
		storage->shader->SetUniformFloat4("u_Color", color);

		storage->vertexArray->Bind();
		RenderCommand::DrawIndexed(storage->vertexArray);
	}
}

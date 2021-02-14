#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Hazel.h>
#include <Hazel/Events/KeyEvent.h>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Hazel::Layer {
private:
	Hazel::ShaderLibrary library;
	
	Hazel::Ref<Hazel::VertexArray> triangleVA;
	Hazel::Ref<Hazel::VertexArray> squareVA;

	Hazel::Ref<Hazel::Texture2D> texture;

	glm::vec3 trianglePosition;
	
	Hazel::OrthographicCameraController cameraController;
	
	float moveSpeed = 0.5f;

	glm::vec3 squareColor = { 0.2f, 0.2f, 0.8f };
	
public:
	ExampleLayer()
		: Layer("Example"), trianglePosition(0.0f), cameraController(1280.0f / 720.0f) {

		triangleVA = Hazel::VertexArray::Create();

		float vertices[3 * 9] = {
			-0.5f, -0.5f, 0.0f,			0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,			0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,			0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->Bind();

		

		//Setup layout
		Hazel::BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		//Layout must de set before
		triangleVA->AddVertexBuffer(vertexBuffer);



		unsigned int indices[3] = { 0, 1, 2 };
		unsigned int indicesSize = sizeof(indices) / sizeof(unsigned int);
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, indicesSize));

		triangleVA->SetIndexBuffer(indexBuffer);



		//Square
		float sqVertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.8f, 0.0f, 1.0f, 1.0f
		};

		squareVA = Hazel::VertexArray::Create();
		//Hazel::Ref<VertexBuffer> squareVB = std::make_shared<VertexBuffer>(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		squareVB->SetLayout(layout);

		squareVA->AddVertexBuffer(squareVB);



		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int squareIndicesSize = sizeof(squareIndices) / sizeof(unsigned int);

		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, squareIndicesSize));

		squareVA->SetIndexBuffer(squareIB);



		//shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));
		//Hazel::Ref<Hazel::Shader> shader = Hazel::Shader::Create("assets/shaders/glsl/Texture.shader");
		Hazel::Ref<Hazel::Shader> textureShader = library.Load("assets/shaders/glsl/Texture.shader");

		
		texture = Hazel::Texture2D::Create("assets/textures/uv_checkerboard.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override {
		//HZ_PRINT_TRACE("Delta time: {0}s", ts);

		//Update
		cameraController.OnUpdate(ts);

		//Todo: Move
		if(Hazel::Input::IsKeyPressed(HZ_KEY_UP)) trianglePosition.y += moveSpeed * ts;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_DOWN)) trianglePosition.y -= moveSpeed * ts;
		if(Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)) trianglePosition.x += moveSpeed * ts;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)) trianglePosition.x -= moveSpeed * ts;
		
		
		//Render
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();

		//Start rendering
		Hazel::Renderer::BeginScene(cameraController.GetCamera());

		/*Hazel::OrthographicCamera cam(1);
		cameraController.GetCamera() = cam;*/

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 colorRed(0.8f, 0.2f, 0.2f, 1.0f);
		glm::vec4 colorGreen(0.2f, 0.8f, 0.2f, 1.0f);
		glm::vec4 colorBlue(0.2f, 0.2f, 0.8f, 1.0f);

		/*Hazel::MaterialRef material = new Hazel::Material(shader);
		Hazel::MaterialInstanceRef mi = new Hazel::MaterialInstanceRef(material);
		mi->Set("u_Color", colorRed);*/

		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->Bind();
		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformFloat3("u_Color", squareColor);
		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformFloat3("u_Color", squareColor);

		
		Hazel::Ref<Hazel::Shader> textureShader = library.Get("Texture");

		
		for(int y = 0; y < 10; y++) {
			for(int x = 0; x < 10; x++) {
				glm::vec3 squarePos(x * 0.12f, y * 0.12f, 0.0f);
				glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), squarePos) * scale;
				Hazel::Renderer::Submit(textureShader, squareVA, squareTransform);
			}
		}

		glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), trianglePosition);
		//Hazel::Renderer::Submit(shader, triangleVA, triangleTransform);

		//Quad
		texture->Bind();
		Hazel::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Hazel::Renderer::EndScene();

		/*if(Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) {
			HZ_PRINT_TRACE("Tab pressed");
		}*/
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override {
		//HZ_PRINT_TRACE("{0}", event)

		cameraController.OnEvent(event);

		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event) {
		//if(event.GetKeyCode() == HZ_KEY_W) {}
		
		return false;
	}
};

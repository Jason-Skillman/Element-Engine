#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Element.h>
#include <Element/Events/KeyEvent.h>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Element::Layer {
private:
	Element::ShaderLibrary library;
	
	Element::Ref<Element::VertexArray> triangleVA;
	Element::Ref<Element::VertexArray> squareVA;

	Element::Ref<Element::Texture2D> texture;

	glm::vec3 trianglePosition;
	
	Element::OrthographicCameraController cameraController;
	
	float moveSpeed = 0.5f;

	glm::vec3 squareColor = { 0.2f, 0.2f, 0.8f };
	
public:
	ExampleLayer()
		: Layer("Example"), trianglePosition(0.0f), cameraController(1280.0f / 720.0f) {

		triangleVA = Element::VertexArray::Create();

		float vertices[3 * 9] = {
			-0.5f, -0.5f, 0.0f,			0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,			0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,			0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		Element::Ref<Element::VertexBuffer> vertexBuffer = Element::VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->Bind();

		

		//Setup layout
		Element::BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		//Layout must de set before
		triangleVA->AddVertexBuffer(vertexBuffer);



		unsigned int indices[3] = { 0, 1, 2 };
		unsigned int indicesSize = sizeof(indices) / sizeof(unsigned int);
		Element::Ref<Element::IndexBuffer> indexBuffer = Element::IndexBuffer::Create(indices, indicesSize);

		triangleVA->SetIndexBuffer(indexBuffer);



		//Square
		float sqVertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.8f, 0.0f, 1.0f, 1.0f
		};

		squareVA = Element::VertexArray::Create();
		//Element::Ref<VertexBuffer> squareVB = std::make_shared<VertexBuffer>(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));

		Element::Ref<Element::VertexBuffer> squareVB = Element::VertexBuffer::Create(sqVertices, sizeof(sqVertices));
		squareVB->SetLayout(layout);

		squareVA->AddVertexBuffer(squareVB);



		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int squareIndicesSize = sizeof(squareIndices) / sizeof(unsigned int);

		Element::Ref<Element::IndexBuffer> squareIB = Element::IndexBuffer::Create(squareIndices, squareIndicesSize);

		squareVA->SetIndexBuffer(squareIB);



		//shader.reset(Element::Shader::Create(vertexSrc, fragmentSrc));
		//Element::Ref<Element::Shader> shader = Element::Shader::Create("assets/shaders/glsl/Texture.shader");
		Element::Ref<Element::Shader> textureShader = library.Load("assets/shaders/glsl/Texture.shader");

		
		texture = Element::Texture2D::Create("assets/textures/uv_checkerboard.png");

		std::dynamic_pointer_cast<Element::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Element::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Element::Timestep ts) override {
		//LOG_TRACE("Delta time: {0}s", ts);

		//Update
		cameraController.OnUpdate(ts);

		//Todo: Move
		if(Element::Input::IsKeyPressed(KEY_UP)) trianglePosition.y += moveSpeed * ts;
		else if(Element::Input::IsKeyPressed(KEY_DOWN)) trianglePosition.y -= moveSpeed * ts;
		if(Element::Input::IsKeyPressed(KEY_RIGHT)) trianglePosition.x += moveSpeed * ts;
		else if(Element::Input::IsKeyPressed(KEY_LEFT)) trianglePosition.x -= moveSpeed * ts;
		
		
		//Render
		Element::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Element::RenderCommand::Clear();

		//Start rendering
		Element::Renderer::BeginScene(cameraController.GetCamera());

		/*Element::OrthographicCamera cam(1);
		cameraController.GetCamera() = cam;*/

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 colorRed(0.8f, 0.2f, 0.2f, 1.0f);
		glm::vec4 colorGreen(0.2f, 0.8f, 0.2f, 1.0f);
		glm::vec4 colorBlue(0.2f, 0.2f, 0.8f, 1.0f);

		/*Element::MaterialRef material = new Element::Material(shader);
		Element::MaterialInstanceRef mi = new Element::MaterialInstanceRef(material);
		mi->Set("u_Color", colorRed);*/

		//std::dynamic_pointer_cast<Element::OpenGLShader>(shader)->Bind();
		//std::dynamic_pointer_cast<Element::OpenGLShader>(shader)->SetUniformFloat3("u_Color", squareColor);
		//std::dynamic_pointer_cast<Element::OpenGLShader>(shader)->SetUniformFloat3("u_Color", squareColor);

		
		Element::Ref<Element::Shader> textureShader = library.Get("Texture");

		
		for(int y = 0; y < 10; y++) {
			for(int x = 0; x < 10; x++) {
				glm::vec3 squarePos(x * 0.12f, y * 0.12f, 0.0f);
				glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), squarePos) * scale;
				Element::Renderer::Submit(textureShader, squareVA, squareTransform);
			}
		}

		glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), trianglePosition);
		//Element::Renderer::Submit(shader, triangleVA, triangleTransform);

		//Quad
		texture->Bind();
		Element::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Element::Renderer::EndScene();

		/*if(Element::Input::IsKeyPressed(KEY_TAB)) {
			LOG_TRACE("Tab pressed");
		}*/
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::End();
	}

	void OnEvent(Element::Event& event) override {
		//LOG_TRACE("{0}", event)

		cameraController.OnEvent(event);

		Element::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Element::KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Element::KeyPressedEvent& event) {
		//if(event.GetKeyCode() == KEY_W) {}
		
		return false;
	}
};

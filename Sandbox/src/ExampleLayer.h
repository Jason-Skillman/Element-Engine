#pragma once

#include "Hazel.h"
#include "imgui.h"

class ExampleLayer : public Hazel::Layer {
private:
	std::shared_ptr<Hazel::Shader> shader;
	std::shared_ptr<Hazel::VertexArray> vertexArray;

	std::shared_ptr<Hazel::VertexArray> squareVA;

	Hazel::OrthographicCamera camera;
	glm::vec3 cameraPosition;
	float cameraRotation;

	float cameraSpeed = 0.01f;
	float cameraRotSpeed = 0.5f;
	
public:
	ExampleLayer()
		: Layer("Example"), camera(1.0f, Hazel::AspectRatio::Ratio16x9), cameraPosition(0), cameraRotation(0) {

		vertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->Bind();


		//Setup layout
		Hazel::BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		//Layout must de set before
		vertexArray->AddVertexBuffer(vertexBuffer);



		unsigned int indices[3] = { 0, 1, 2 };
		unsigned int indicesSize = sizeof(indices) / sizeof(unsigned int);
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, indicesSize));

		vertexArray->SetIndexBuffer(indexBuffer);



		//Square
		//Todo: Move
		float sqVertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.8f, 0.0f, 1.0f, 1.0f
		};

		squareVA.reset(Hazel::VertexArray::Create());
		//std::shared_ptr<VertexBuffer> squareVB = std::make_shared<VertexBuffer>(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		squareVB->SetLayout(layout);

		squareVA->AddVertexBuffer(squareVB);



		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int squareIndicesSize = sizeof(squareIndices) / sizeof(unsigned int);

		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, squareIndicesSize));

		squareVA->SetIndexBuffer(squareIB);




		//Create shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
		
			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		
				v_Position = a_Position;
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 o_Color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				//o_Color = vec4(0.8, 0.2, 0.3, 1.0);
				//o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
				o_Color = v_Color;
			}
		)";

		shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override {
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();


		//Move camera
		if(Hazel::Input::IsKeyPressed(HZ_KEY_W)) cameraPosition.y += cameraSpeed;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_S)) cameraPosition.y -= cameraSpeed;
		if(Hazel::Input::IsKeyPressed(HZ_KEY_D)) cameraPosition.x += cameraSpeed;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_A)) cameraPosition.x -= cameraSpeed;
		if(Hazel::Input::IsKeyPressed(HZ_KEY_E)) cameraRotation -= cameraRotSpeed;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_Q)) cameraRotation += cameraRotSpeed;

		camera.SetPosition(cameraPosition);
		camera.SetRotation(cameraRotation);


		//Start rendering
		Hazel::Renderer::BeginScene(camera);

		Hazel::Renderer::Submit(shader, squareVA);

		Hazel::Renderer::EndScene();

		/*if(Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) {
			HZ_PRINT_TRACE("Tab pressed");
		}*/
	}

	void OnImGuiRender() override {
		/*ImGui::Begin("Test");
		ImGui::Text("Hello");
		ImGui::End();*/
	}

	void OnEvent(Hazel::Event& event) override {
		//HZ_PRINT_TRACE("{0}", event)

		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event) {
		//if(event.GetKeyCode() == HZ_KEY_W) {}
		
		return false;
	}
};

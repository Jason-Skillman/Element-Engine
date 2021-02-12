#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h> 

#include "Platform/OpenGL/OpenGLShader.h"

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(1280.0f / 720.0f) { }

void SandboxLayer::OnAttach() {
	float sqVertices[4 * 9] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.8f, 0.0f, 1.0f, 1.0f
	};

	Hazel::BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" }
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


	colorShader = Hazel::Shader::Create("assets/shaders/glsl/UniColor.shader");
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Hazel::Timestep ts) {
	//Update
	cameraController.OnUpdate(ts);

	//Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hazel::RenderCommand::Clear();

	//Start rendering
	Hazel::Renderer::BeginScene(cameraController.GetCamera());

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(colorShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(colorShader)->SetUniformFloat4("u_Color", squareColor);

	Hazel::Renderer::Submit(colorShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Hazel::Renderer::EndScene();
}

void SandboxLayer::OnImGuiRender() {
	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

void SandboxLayer::OnEvent(Hazel::Event& event) {
	cameraController.OnEvent(event);
}

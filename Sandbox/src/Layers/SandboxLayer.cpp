#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h> 

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(1280.0f / 720.0f) { }

void SandboxLayer::OnAttach() {
	
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Hazel::Timestep ts) {
	//Update
	cameraController.OnUpdate(ts);

	//Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hazel::RenderCommand::Clear();

	//Start rendering
	Hazel::Renderer2D::BeginScene(cameraController.GetCamera());

	//Todo: Change to Shader::SetMat4()
	//std::dynamic_pointer_cast<Hazel::OpenGLShader>(colorShader)->Bind();
	//std::dynamic_pointer_cast<Hazel::OpenGLShader>(colorShader)->SetUniformFloat4("u_Color", squareColor);

	//Hazel::Renderer::Submit(colorShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	Hazel::Renderer2D::DrawQuad(glm::vec2(0, 0), glm::vec2(1, 1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	Hazel::Renderer2D::EndScene();
}

void SandboxLayer::OnImGuiRender() {
	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

void SandboxLayer::OnEvent(Hazel::Event& event) {
	cameraController.OnEvent(event);
}

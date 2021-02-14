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

	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f}, 15.0f, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ -0.5f, -0.5f}, 15.0f, { 0.8f, 0.5f }, { 0.2f, 0.8f, 0.3f, 1.0f });

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

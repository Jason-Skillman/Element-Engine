#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { profileResults.push_back(profileResult); })

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(1280.0f / 720.0f) { }

void SandboxLayer::OnAttach() {
	textureCheckerboard = Hazel::Texture2D::Create("assets/textures/checkerboard.png");
	textureArrow = Hazel::Texture2D::Create("assets/textures/arrow_head.png");
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Hazel::Timestep ts) {
	HZ_PROFILE_FUNCTION();
	
	//Update
	{
		HZ_PROFILE_SCOPE("Update");
		
		cameraController.OnUpdate(ts);
	}

	//Pre-render
	{
		HZ_PROFILE_SCOPE("Pre-render");
		
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	//Rendering
	{
		HZ_PROFILE_SCOPE("Rendering");

		Hazel::Renderer2D::BeginScene(cameraController.GetCamera());

		//Red square
		{
			Hazel::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.1f };
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.8f, 0.2f, 0.3f, 1.0f };
			Hazel::Renderer2D::DrawQuad(drawProps);
		}

		//Green square
		{
			static float rotation = 0.0f;
			rotation += ts * 20.0f;
			
			Hazel::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.6f, 0.0f, 0.2f };
			drawProps.rotation = rotation;
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.3f, 0.8f, 0.2f, 1.0f };
			Hazel::Renderer2D::DrawQuad(drawProps);
		}

		//Blue square
		{
			static float rotation = 0.0f;
			rotation += ts * 45.0f;
			
			Hazel::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 1.2f, 0.0f, 0.3f };
			drawProps.rotation = rotation;
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.2f, 0.3f, 0.8f, 1.0f };
			Hazel::Renderer2D::DrawQuad(drawProps);
		}

		//Texture
		{
			Hazel::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.4f };
			drawProps.scale = { 0.3f, 0.3f };
			drawProps.texture = textureArrow;
			Hazel::Renderer2D::DrawQuad(drawProps);
		}

		//Background
		{
			Hazel::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.0f };
			drawProps.scale = { 10.0f, 10.0f };
			drawProps.texture = textureCheckerboard;
			drawProps.tiling = 20.0f;
			Hazel::Renderer2D::DrawQuad(drawProps);
		}
		
		Hazel::Renderer2D::EndScene();
	}
}

void SandboxLayer::OnImGuiRender() {
	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();

	/*ImGui::Begin("Profiler");

	for(auto& result : profileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	profileResults.clear();

	ImGui::End();*/
}

void SandboxLayer::OnEvent(Hazel::Event& event) {
	cameraController.OnEvent(event);
}

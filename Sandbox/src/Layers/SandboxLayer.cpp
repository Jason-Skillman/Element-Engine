#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "../../../Element/vendor/GLFW/include/GLFW/glfw3.h"

#include "Element/Debug/Instrumentor.h"

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(16.0f / 9.0f) {}

void SandboxLayer::OnAttach() {
	textureCheckerboard = Element::Texture2D::Create("assets/textures/checkerboard.png");
	textureArrow = Element::Texture2D::Create("assets/textures/arrow_head.png");

	particleProps.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	particleProps.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	particleProps.SizeBegin = 0.5f, particleProps.SizeVariation = 0.3f, particleProps.SizeEnd = 0.0f;
	particleProps.LifeTime = 1.0f;
	particleProps.Velocity = { 0.0f, 0.0f };
	particleProps.VelocityVariation = { 3.0f, 1.0f };
	particleProps.Position = { 0.0f, 0.0f };
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Element::Timestep ts) {
	PROFILE_FUNCTION();
	
	//Update
	{
		PROFILE_SCOPE("Update");
		
		cameraController.OnUpdate(ts);
	}

	//Pre-render
	{
		PROFILE_SCOPE("Pre-render");
		
		Element::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Element::RenderCommand::Clear();
	}

	//Rendering
	{
		PROFILE_SCOPE("Rendering");

		Element::Renderer2D::ResetStats();
		Element::Renderer2D::BeginScene(cameraController.GetCamera());

		//Red square
		{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.1f };
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.8f, 0.2f, 0.3f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}

		//Green square
		{
			static float rotation = 0.0f;
			rotation += ts * 20.0f;
			
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.6f, 0.0f, 0.2f };
			drawProps.rotation = glm::radians(rotation);
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.3f, 0.8f, 0.2f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}

		//Blue square
		{
			static float rotation = 0.0f;
			rotation += ts * 45.0f;
			
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 1.2f, 0.0f, 0.3f };
			drawProps.rotation = glm::radians(rotation);
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.2f, 0.3f, 0.8f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}

		//Texture
		{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.4f };
			drawProps.scale = { 0.3f, 0.3f };
			drawProps.texture = textureArrow;
			Element::Renderer2D::DrawQuad(drawProps);
		}

		//Background
		{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, -0.1f };
			drawProps.scale = { 10.0f, 10.0f };
			drawProps.texture = textureCheckerboard;
			drawProps.tiling = 20.0f;
			Element::Renderer2D::DrawQuad(drawProps);
		}
		
		Element::Renderer2D::EndScene();


		if(Element::Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			auto [x, y] = Element::Input::GetMousePosition();
			auto width = Element::Application::GetInstance().GetWindow().GetWidth();
			auto height = Element::Application::GetInstance().GetWindow().GetHeight();

			auto bounds = cameraController.GetBounds();
			auto pos = cameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			particleProps.Position = { x + pos.x, y + pos.y };
			for(int i = 0; i < 5; i++)
				particleSystem.Emit(particleProps);
		}

		particleSystem.OnUpdate(ts);
		particleSystem.OnRender(cameraController.GetCamera());
	}
}

void SandboxLayer::OnImGuiRender() {
	/*ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();*/

	/*ImGui::Begin("Profiler");

	for(auto& result : profileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	profileResults.clear();

	ImGui::End();*/

	ImGui::Begin("Renderer 2D Stats");

	auto stats = Element::Renderer2D::GetStats();
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void SandboxLayer::OnEvent(Element::Event& event) {
	cameraController.OnEvent(event);
}

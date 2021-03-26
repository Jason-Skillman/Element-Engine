#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(16.0f / 9.0f) {}

void SandboxLayer::OnAttach() {
	textureCheckerboard = Element::Texture2D::Create("assets/textures/checkerboard.png");
	textureArrow = Element::Texture2D::Create("assets/textures/arrow_head.png");

	Element::FrameBufferProperties fbProps;
	fbProps.width = 1280;
	fbProps.height = 720;
	
	particleProps.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	particleProps.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	particleProps.sizeBegin = 0.5f, particleProps.sizeVariation = 0.3f, particleProps.sizeEnd = 0.0f;
	particleProps.lifeTime = 1.0f;
	particleProps.velocity = { 0.0f, 0.0f };
	particleProps.velocityVariation = { 3.0f, 1.0f };
	particleProps.position = { 0.0f, 0.0f };
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Element::Timestep ts) {
	EL_PROFILE_FUNCTION();
	
	//Update
	{
		EL_PROFILE_SCOPE("Update");
		
		cameraController.OnUpdate(ts);
	}

	//Pre-render
	{
		EL_PROFILE_SCOPE("Pre-render");

		Element::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Element::RenderCommand::Clear();
	}

	//Rendering
	{
		EL_PROFILE_SCOPE("Rendering");

		Element::Renderer2D::ResetStats();
		Element::Renderer2D::BeginScene(cameraController.GetCamera());

		//Red square
		{
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.1f };
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.8f, 0.2f, 0.3f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}

		//Green square
		{
			static float rotation = 0.0f;
			rotation += ts * 20.0f;
			
			Element::Renderer2D::DrawProperties drawProps;
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
			
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { 1.2f, 0.0f, 0.3f };
			drawProps.rotation = glm::radians(rotation);
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.2f, 0.3f, 0.8f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}

		//Arrow texture
		{
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.4f };
			drawProps.scale = { 0.3f, 0.3f };
			Element::Renderer2D::DrawQuad(drawProps, textureArrow);
		}

		//Background
		{
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { 0.0f, 0.0f, -0.1f };
			drawProps.scale = { 10.0f, 10.0f };
			drawProps.tiling = 20.0f;
			Element::Renderer2D::DrawQuad(drawProps, textureCheckerboard);
		}
		
		Element::Renderer2D::EndScene();

		//Particles
		if(Element::Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			auto [x, y] = Element::Input::GetMousePosition();
			auto width = Element::Application::GetInstance().GetWindow().GetWidth();
			auto height = Element::Application::GetInstance().GetWindow().GetHeight();

			auto bounds = cameraController.GetBounds();
			auto pos = cameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			particleProps.position = { x + pos.x, y + pos.y };
			particleProps.sizeBegin = 0.1f;
			for(int i = 0; i < 5; i++)
				particleSystem.Emit(particleProps);
		}

		particleSystem.OnUpdate(ts);
		particleSystem.OnRender(cameraController.GetCamera());
	}
}

void SandboxLayer::OnImGuiRender() {
	ImGui::Begin("Renderer2D Stats");

	auto stats = Element::Renderer2D::GetStats();
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Textures loaded: %d", stats.texturesLoaded);

	ImGui::End();
}

void SandboxLayer::OnEvent(Element::Event& event) {
	cameraController.OnEvent(event);
}

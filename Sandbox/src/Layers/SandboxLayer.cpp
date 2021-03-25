#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(16.0f / 9.0f) {}

void SandboxLayer::OnAttach() {
	textureCheckerboard = Element::Texture2D::Create("assets/textures/checkerboard.png");
	textureArrow = Element::Texture2D::Create("assets/textures/arrow_head.png");
	
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

		//Arrow texture
		{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.4f };
			drawProps.scale = { 0.3f, 0.3f };
			Element::Renderer2D::DrawQuad(drawProps, textureArrow);
		}

		//Background
		{
			Element::Renderer2D::DrawProporties drawProps;
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
	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if(opt_fullscreen) {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	} else {
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if(!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if(!opt_padding)
		ImGui::PopStyleVar();

	if(opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("File")) {
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			//ImGui::Separator();

			if(ImGui::MenuItem("Exit")) {
				Element::Application::GetInstance().Close();
			}

			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}

	{
		ImGui::Begin("Renderer2D Stats");

		auto stats = Element::Renderer2D::GetStats();
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Textures loaded: %d", stats.texturesLoaded);

		uint32_t textureID = textureCheckerboard->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 64.0f, 64.0f });

		ImGui::End();
	}

	ImGui::End();
	
}

void SandboxLayer::OnEvent(Element::Event& event) {
	cameraController.OnEvent(event);
}

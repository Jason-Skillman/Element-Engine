#include "pch.h"
#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Element {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), cameraController(16.0f / 9.0f) {}

	void EditorLayer::OnAttach() {
		textureCheckerboard = Texture2D::Create("assets/textures/checkerboard.png");
		textureArrow = Texture2D::Create("assets/textures/arrow_head.png");

		FrameBufferProperties fbProps;
		fbProps.width = 1280;
		fbProps.height = 720;
		frameBuffer = FrameBuffer::Create(fbProps);


		//ECS
		activeScene = CreateRef<Scene>();

		entityCamera = activeScene->CreateEntity("Camera");
		entityCamera.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		
		entitySquare = activeScene->CreateEntity();
		entitySquare.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
	}

	void EditorLayer::OnDetach() {}

	void EditorLayer::OnUpdate(Timestep ts) {
		EL_PROFILE_FUNCTION();

		//Update
		{
			EL_PROFILE_SCOPE("Update");

			if(viewportFocused) {
				cameraController.OnUpdate(ts);
			}
		}

		//Pre-render
		{
			EL_PROFILE_SCOPE("Pre-render");

			frameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
		}

		//Rendering
		{
			EL_PROFILE_SCOPE("Rendering");

			Renderer2D::ResetStats();
			
			activeScene->OnUpdate(ts);
			frameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender() {
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
					Application::GetInstance().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		{
			ImGui::Begin("Renderer2D Stats");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Draw calls: %d", stats.drawCalls);
			ImGui::Text("Quads: %d", stats.quadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::Text("Textures loaded: %d", stats.texturesLoaded);
			ImGui::Separator();

			if(entitySquare) {
				ImGui::Text("%s", entitySquare.GetComponent<TagComponent>().tag.c_str());
			}

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");

			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			Application::GetInstance().GetImGuiLayer()->SetBlockEvents(!viewportFocused || !viewportHovered);
			
			//Set the viewport size
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			
			if(viewportSize != *reinterpret_cast<glm::vec2*>(&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				frameBuffer->Resize(viewportSize.x, viewportSize.y);

				cameraController.OnResize(viewportSize.x, viewportSize.y);
			}

			//Draw the frame buffer
			uint32_t textureID = frameBuffer->GetColorAttachmentRendererId();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& event) {
		cameraController.OnEvent(event);
	}
}

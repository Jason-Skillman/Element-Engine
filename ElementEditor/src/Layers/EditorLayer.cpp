#include "pch.h"
#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

#include "Element/Codes/KeyCodes.h"
#include "Element/Core/Core.h"
#include "Element/Scene/Scene.h"
#include "Element/Scene/SceneSerializer.h"
#include "Element/Utils/PlatformUtils.h"
#include "Element/Math/Math.h"

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

#if 0
		entityCamera = activeScene->CreateEntity("Camera");
		entityCamera.AddComponent<CameraComponent>();
		
		entityRedSquare = activeScene->CreateEntity("Red Square");
		entityRedSquare.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

		entityGreenSquare = activeScene->CreateEntity("Green Square");
		entityGreenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });


		//Todo: Temp
		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() override {}
			void OnDestroy() override {}
			void OnUpdate(Timestep ts) override {
				auto& translation = GetComponent<TransformComponent>().translation;
				float speed = 5.0f;

				if(Input::IsKeyPressed(KEY_A)) {
					translation.x -= speed * ts;
				}
				else if(Input::IsKeyPressed(KEY_D)) {
					translation.x += speed * ts;
				}
				if(Input::IsKeyPressed(KEY_W)) {
					translation.y += speed * ts;
				}
				else if(Input::IsKeyPressed(KEY_S)) {
					translation.y -= speed * ts;
				}
			}
		};

		entityCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		sceneHierarchyPanel.SetContext(activeScene);
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

		ImGuiStyle& style = ImGui::GetStyle();
		const float originalMinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = originalMinSize;


		//----- Setup the menu bar -----
		if(ImGui::BeginMenuBar()) {

			if(ImGui::BeginMenu("File")) {
				
				if(ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if(ImGui::MenuItem("Open", "Ctrl+O"))
					OpenScene();
				
				if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();
				
				if(ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();

				ImGui::EndMenu();
			}

			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			//ImGui::Separator();

			//if(ImGui::BeginMenu("Debug")) {
			//	/*if(ImGui::BeginMenu("Nest2")) {
			//		if(ImGui::MenuItem("Item1")) {

			//		}
			//		ImGui::EndMenu();
			//	}*/
			//	ImGui::EndMenu();
			//}

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

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");

			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			Application::GetInstance().GetImGuiLayer()->SetBlockEvents(!viewportFocused && !viewportHovered);
			
			//Set the viewport size
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

			if(viewportSize != *reinterpret_cast<glm::vec2*>(&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				frameBuffer->Resize(viewportSize.x, viewportSize.y);

				cameraController.OnResize(viewportSize.x, viewportSize.y);

				activeScene->OnViewportResize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
			}

			//Draw the frame buffer
			uint32_t textureID = frameBuffer->GetColorAttachmentRendererId();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			//Draw gizmos
			{
				Entity selectedEntity = sceneHierarchyPanel.GetSelectedEntity();

				if(selectedEntity && gizmoType != -1) {
					ImGuizmo::SetDrawlist();

					const float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
					const float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

					//Camera
					Entity cameraEntity = activeScene->GetPrimaryCameraEntity();
					const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
					const glm::mat4& cameraProjection = camera.GetProjection();
					glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

					//Change the gizmo's projection based on the camera
					const SceneCamera::ProjectionType projectionType = camera.GetProjectionType();
					ImGuizmo::SetOrthographic(projectionType == SceneCamera::ProjectionType::Orthographic ? true : false);

					//Entity transform
					auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = transformComponent.GetTransform();

					//Snapping
					bool snap = Input::IsKeyPressed(KEY_LEFT_CONTROL);
					float snapValue = 0.5f;	//Snap value in meters
					if(gizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					//DO NOT REMOVE!
					//For some reason ImGuizmo::Manipulate() sets the transform matrix to NaN when matrix is set to identity. (0, 0, scale is 1)
					//This offsets the gizmo from the zero origin just enough to prevent the NaN.
					//EL_ASSERT(!std::isnan(transform[0][0]), "NAN");
					if((transform[0][1] == 0 && transform[0][2] == 0 && transform[0][3] == 0 &&
						transform[1][0] == 0 && transform[1][2] == 0 && transform[1][3] == 0 &&
						transform[2][0] == 0 && transform[2][1] == 0 && transform[2][3] == 0 &&
						transform[3][0] == 0 && transform[3][1] == 0 && transform[3][2] == 0)) {

						constexpr float zeroOffset = 0.0000001f;

						transform[3][0] += zeroOffset;
						transform[3][1] += zeroOffset;
						transform[3][2] += zeroOffset;
					}

					//Render the gizmo
					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapValues : nullptr);

					
					if(ImGuizmo::IsUsing() && !std::isnan(transform[0][0])) {
						//Split mat4 transform into translation, rotation, scale
						glm::vec3 translation = {}, rotation = {}, scale = {};

						Math::DecomposeTransform(transform, translation, rotation, scale);

						//Apply the gizmo transformation
						if(gizmoType == ImGuizmo::OPERATION::TRANSLATE)
							transformComponent.translation = translation;
						if(gizmoType == ImGuizmo::OPERATION::ROTATE) {
							glm::vec3 deltaRotation = rotation - transformComponent.rotation;	//delta rotation = decomposed rotation - original rotation
							transformComponent.rotation += deltaRotation;
						}
						if(gizmoType == ImGuizmo::OPERATION::SCALE)
							transformComponent.scale = scale;
					}
				}

				ImGui::End();
				ImGui::PopStyleVar();
			}
		}

		sceneHierarchyPanel.OnImGuiRender();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event) {
		cameraController.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EL_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::NewScene() {
		activeScene = CreateRef<Scene>();
		activeScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchyPanel.SetContext(activeScene);
	}

	void EditorLayer::OpenScene() {
		std::string filePath = FileDialog::OpenFile("Element Scene (*.element)\0*.element\0");

		if(!filePath.empty()) {
			NewScene();

			SceneSerializer serializer(activeScene);
			serializer.Deserialize(filePath);
		}
	}

	void EditorLayer::SaveSceneAs() {
		std::string filePath = FileDialog::SaveFile("Element Scene (*.element)\0*.element\0");

		if(!filePath.empty()) {
			SceneSerializer serializer(activeScene);
			serializer.Serialize(filePath);
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
		//Shortcuts
		if(event.GetRepeatCount() > 0) return false;

		const bool control = Input::IsKeyPressed(KEY_LEFT_CONTROL) || Input::IsKeyPressed(KEY_RIGHT_CONTROL);
		const bool shift = Input::IsKeyPressed(KEY_LEFT_SHIFT) || Input::IsKeyPressed(KEY_RIGHT_SHIFT);

		switch(event.GetKeyCode()) {
			//Editor shortcuts
			case KEY_N:
				if(control) NewScene();
				break;
			case KEY_O:
				if(control) OpenScene();
				break;
			case KEY_S:
				if(control && shift) SaveSceneAs();
				break;
			
			//Gizmo shortcuts
			case KEY_Q:	//Selection
				gizmoType = -1;
				break;
			case KEY_W:	//Translate
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case KEY_E:	//Rotate
				gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case KEY_R:	//Scale
				gizmoType = ImGuizmo::OPERATION::SCALE;
				break;

			default:
				break;
		}
		return false;
	}
}

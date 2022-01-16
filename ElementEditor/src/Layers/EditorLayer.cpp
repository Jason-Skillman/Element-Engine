#include "pch.h"
#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

#include "Element/Codes/FontTypes.h"
#include "Element/Codes/KeyCodes.h"
#include "Element/Core/Core.h"
#include "Element/Scene/Scene.h"
#include "Element/Scene/SceneSerializer.h"
#include "Element/Utils/PlatformUtils.h"
#include "Element/Math/Math.h"

namespace Element {
	
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), cameraController(16.0f / 9.0f), editorCamera(45.f, 16.0 / 9.0f, 0.1f, 1000.0f) {}

	void EditorLayer::OnAttach() {
		EL_PROFILE_FUNCTION();

		textureCheckerboard = Texture2D::Create("Assets/Textures/Checkerboard.png");
		textureArrow = Texture2D::Create("Assets/Textures/arrow_head.png");

		FrameBufferSpecification fbSpec;
		fbSpec.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		frameBuffer = FrameBuffer::Create(fbSpec);

		activeScene = CreateRef<Scene>();

		//Command args
		auto commandLineArgs = Application::GetInstance().GetCommandLineArgs();
		if(commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			EL_LOG_CORE_INFO("Loading scene at: {0}", sceneFilePath);
			SceneSerializer serializer(activeScene);
			serializer.Deserialize(sceneFilePath);
		}

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

	void EditorLayer::OnDetach() {
		EL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		EL_PROFILE_FUNCTION();

		//Update
		{
			EL_PROFILE_SCOPE("Update");

			if(viewportFocused)
				cameraController.OnUpdate(ts);
			
			editorCamera.OnUpdate(ts);
		}

		//Rendering
		{
			EL_PROFILE_SCOPE("Rendering");

			Renderer2D::ResetStats();

			frameBuffer->Bind();

			//Pre-render
			{
				EL_PROFILE_SCOPE("Pre-render");

				RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
				RenderCommand::Clear();
			}

			//Clear enity ID attachment to -1
			frameBuffer->ClearAttachment(1, -1);
			
			activeScene->OnUpdateEditor(ts, editorCamera);

			auto [mx, my] = ImGui::GetMousePos();
			mx -= viewportBounds[0].x;	//Makes it 0, 0
			my -= viewportBounds[0].y;	//Makes it 0, 0
			glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
			
			//Top left is (0, 0)
			
			//Flip y to make it bottom left (0, 0)
			//OpenGl/textures like (0, 0) on the bottom left
			my = viewportSize.y - my;

			int mouseX = static_cast<int>(mx);
			int mouseY = static_cast<int>(my);

			//Check if mouse is within viewport bounds
			if(mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY < static_cast<int>(viewportSize.y)) {
				//EL_LOG_CORE_TRACE("Viewport mouse pos: {0}, {1}", mouseX, mouseY);

				int value = frameBuffer->ReadPixel(1, mouseX, mouseY);
				hoveredEntity = value != -1 ? Entity(static_cast<entt::entity>(value), activeScene.get()) : Entity();
			}

			frameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender() {
		EL_PROFILE_FUNCTION();

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

		//Fonts
		ImFont* boldFont = io.Fonts->Fonts[static_cast<int>(FontType_Bold)];


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

			if(ImGui::BeginMenu("Window")) {

				if(ImGui::MenuItem("Settings")) {
					showSettingsWindow = true;
					ImGui::SetWindowFocus("Settings");
				}

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
			if(showSettingsWindow) {

				if(!ImGui::Begin("Settings", &showSettingsWindow)) {
					ImGui::End();
				} else {
					ImGui::Text("Test");

					ImGui::End();
				}
			}
		}

		{
			ImGui::Begin("Stats");

			{
				ImGui::PushFont(boldFont);
				ImGui::Text("Entities");
				ImGui::PopFont();
				std::string entityName = hoveredEntity ? hoveredEntity.GetComponent<TagComponent>().tag : "None";
				ImGui::Text("Hovered Entity: %s", entityName.c_str());
				ImGui::Separator();
			}

			{
				ImGui::PushFont(boldFont);
				ImGui::Text("Renderer2D");
				ImGui::PopFont();
				Renderer2D::Statistics stats = Renderer2D::GetStats();
				ImGui::Text("Draw calls: %d", stats.drawCalls);
				ImGui::Text("Quads: %d", stats.quadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
				ImGui::Text("Textures loaded: %d", stats.texturesLoaded);
				ImGui::Separator();
			}

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::Begin("Viewport");

			ImVec2 viewportOffset = ImGui::GetCursorPos();	//Includes the tab bar
			//viewportOffset.y -= ImGui::GetFontSize() + ImGui::frame() * 2;

			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			Application::GetInstance().GetImGuiLayer()->SetBlockEvents(!viewportFocused && !viewportHovered);

			//Set the viewport size
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

			if(viewportSize != *reinterpret_cast<glm::vec2*>(&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				frameBuffer->Resize(viewportSize.x, viewportSize.y);

				cameraController.OnResize(viewportSize.x, viewportSize.y);
				editorCamera.SetViewportSize(viewportSize.x, viewportSize.y);

				activeScene->OnViewportResize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
			}

			//Draw the frame buffer
			uint32_t textureID = frameBuffer->GetColorAttachmentID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			//Get the bounds of the window. Excludes the window's title bar.
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 minBound = ImGui::GetWindowContentRegionMin();
			ImVec2 maxBound = ImGui::GetWindowContentRegionMax();

			minBound.x += windowPos.x;
			minBound.y += windowPos.y;
			maxBound.x += windowPos.x;
			maxBound.y += windowPos.y;

			//Min will be stored in the first slot, max in the second
			viewportBounds[0] = { minBound.x, minBound.y };
			viewportBounds[1] = { maxBound.x, maxBound.y };

			//Draw gizmos
			{
				Entity selectedEntity = sceneHierarchyPanel.GetSelectedEntity();

				if(selectedEntity && gizmoType != -1) {
					ImGuizmo::SetDrawlist();

					const float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
					const float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

					//Camera
					//Runtime camera
					/*Entity cameraEntity = activeScene->GetPrimaryCameraEntity();
					const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
					const glm::mat4& cameraProjection = camera.GetProjection();
					glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

					//Editor camera
					const glm::mat4& cameraProjection = editorCamera.GetProjection();
					glm::mat4 cameraView = editorCamera.GetViewMatrix();

					//Change the gizmo's projection based on the camera
					const ProjectionType projectionType = editorCamera.GetProjectionType();
					ImGuizmo::SetOrthographic(projectionType == ProjectionType::Orthographic ? true : false);

					//Entity transform
					TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = transformComponent.GetTransform();

					//Snapping
					bool snap = Input::IsKeyPressed(Key::LeftControl);
					float snapValue = 0.5f;	//Snap value in meters
					if(gizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					//DO NOT REMOVE!
					//For some reason ImGuizmo::Manipulate() sets the transform matrix to NaN when matrix is set to identity. { 0, 0, 0 } scale is 1
					//This offsets the gizmo from the zero origin just enough to prevent the NaN.
					//EL_ASSERT(!std::isnan(transform[0][0]), "NAN");
					{
						constexpr float zeroOffset = 0.0000001f;

						if(transform[3][0] == 0)
							transform[3][0] += zeroOffset;
						if(transform[3][1] == 0)
							transform[3][1] += zeroOffset;
						if(transform[3][2] == 0)
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
			}

			ImGui::End();
			ImGui::PopStyleVar();
		}

		sceneHierarchyPanel.OnImGuiRender();
		contentBrowserPanel.OnImGuiRender();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event) {
		EL_PROFILE_FUNCTION();

		cameraController.OnEvent(event);
		editorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EL_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(EL_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));
	}

	void EditorLayer::NewScene() {
		EL_PROFILE_FUNCTION();

		activeScene = CreateRef<Scene>();
		activeScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		sceneHierarchyPanel.SetContext(activeScene);
	}

	void EditorLayer::OpenScene() {
		EL_PROFILE_FUNCTION();

		std::string filePath = FileDialog::OpenFile("Element Scene (*.scene)\0*.scene\0");

		if(!filePath.empty()) {
			NewScene();

			SceneSerializer serializer(activeScene);
			serializer.Deserialize(filePath);
		}
	}

	void EditorLayer::SaveSceneAs() {
		EL_PROFILE_FUNCTION();

		std::string filePath = FileDialog::SaveFile("Element Scene (*.scene)\0*.scene\0");

		if(!filePath.empty()) {
			SceneSerializer serializer(activeScene);
			serializer.Serialize(filePath);
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
		EL_PROFILE_FUNCTION();

		//Shortcuts
		if(event.GetRepeatCount() > 0) return false;

		const bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		const bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch(event.GetKeyCode()) {
			//Editor shortcuts
			case Key::N:
				if(control) NewScene();
				break;
			case Key::O:
				if(control) OpenScene();
				break;
			case Key::S:
				if(control && shift) SaveSceneAs();
				break;
			
			//Gizmo shortcuts
			case Key::Q:	//Selection
				gizmoType = -1;
				break;
			case Key::W:	//Translate
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:	//Rotate
				gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:	//Scale
				gizmoType = ImGuizmo::OPERATION::SCALE;
				break;

			default:
				break;
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
		EL_PROFILE_FUNCTION();

		//Select the hovered entity on mouse click
		if(event.GetMouseButton() == Mouse::ButtonLeft && viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
			sceneHierarchyPanel.SetSelectedEntity(hoveredEntity);

		return false;
	}
}

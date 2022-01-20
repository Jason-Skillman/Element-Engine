#pragma once

#include "Element.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Element {

	class EditorLayer : public Layer {
	private:
		enum class SceneState {
			Edit = 0,
			Play = 1,
		};
		
	private:
		EditorCamera editorCamera;
		OrthographicCameraController cameraController;

		Ref<Texture2D> iconPlay, iconStop, textureCheckerboard, textureArrow;

		Ref<Scene> activeScene;
		Entity entityCamera;
		Entity entityRedSquare, entityGreenSquare;
		Entity hoveredEntity;

		Ref<FrameBuffer> frameBuffer;
		glm::vec2 viewportSize = { 0, 0 };
		glm::vec2 viewportBounds[2] = { glm::vec2(0), glm::vec2(0)};
		bool viewportFocused = false, viewportHovered = false;

		int gizmoType = -1;

		SceneState sceneState = SceneState::Edit;

		//Panels
		SceneHierarchyPanel sceneHierarchyPanel;
		ContentBrowserPanel contentBrowserPanel;

		//Windows
		bool showSettingsWindow = false;

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		void UIToolbar();

		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
	};
}

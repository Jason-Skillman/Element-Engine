#pragma once

#include "Element.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Element {

	class EditorLayer : public Layer {
	private:
		EditorCamera editorCamera;
		OrthographicCameraController cameraController;

		Ref<Texture2D> textureCheckerboard, textureArrow;

		Ref<Scene> activeScene;
		Entity entityCamera;
		Entity entityRedSquare, entityGreenSquare;
		Entity hoveredEntity;

		Ref<FrameBuffer> frameBuffer;
		glm::vec2 viewportSize = { 0, 0 };
		glm::vec2 viewportBounds[2];
		bool viewportFocused = false, viewportHovered = false;

		int gizmoType = -1;

		//Panels
		SceneHierarchyPanel sceneHierarchyPanel;

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
		void SaveSceneAs();

		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
	};
}

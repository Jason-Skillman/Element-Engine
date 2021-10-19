#pragma once

#include "Element.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Element {
	class EditorLayer : public Layer {
	private:
		OrthographicCameraController cameraController;

		Ref<Texture2D> textureCheckerboard, textureArrow;

		Ref<Scene> activeScene;
		Entity entityCamera;
		Entity entityRedSquare, entityGreenSquare;

		Ref<FrameBuffer> frameBuffer;
		glm::vec2 viewportSize = { 0, 0 };
		bool viewportFocused = false, viewportHovered = false;

		//Panels
		SceneHierarchyPanel sceneHierarchyPanel;

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	};
}

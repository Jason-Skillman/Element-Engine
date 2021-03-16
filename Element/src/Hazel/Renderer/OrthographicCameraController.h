#pragma once

#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel {
	class OrthographicCameraController {
	private:
		float aspectRatio;
		float zoomLevel = 1.0f;
		OrthographicCamera camera;

		glm::vec3 cameraPosition;
		float cameraRotation;

		float cameraSpeed = 0.5f, cameraRotSpeed = 20.0f;

	public:
		OrthographicCameraController(float aspectRatio);

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		inline void SetZoomLevel(float zoomLevel) {
			this->zoomLevel = zoomLevel;
		}
		inline float GetZoomLevel() const {
			return zoomLevel;
		}

		inline OrthographicCamera& GetCamera() {
			return camera;
		}
		inline const OrthographicCamera& GetCamera() const {
			return camera;
		}

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
	};
}

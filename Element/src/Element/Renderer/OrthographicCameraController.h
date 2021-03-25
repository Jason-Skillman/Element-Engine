#pragma once

#include "Element/Core/Timestep.h"

#include "Element/Events/ApplicationEvent.h"
#include "Element/Events/MouseEvent.h"

#include "Element/Renderer/OrthographicCamera.h"

namespace Element {
	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};
	
	class OrthographicCameraController {
	private:
		float zoomLevel = 1.0f;
		float aspectRatio;
		OrthographicCameraBounds bounds;
		OrthographicCamera camera;

		glm::vec3 cameraPosition;
		float cameraRotation;

		float moveSpeed = 1.0f, rotationSpeed = 30.0f;

	public:
		OrthographicCameraController(float aspectRatio);

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		inline void SetZoomLevel(float zoomLevel) {
			this->zoomLevel = zoomLevel;
			CalculateView();
		}
		inline float GetZoomLevel() const {
			return zoomLevel;
		}

		inline void SetMoveSpeed(float speed) {
			moveSpeed = speed;
		}
		inline float GetMoveSpeed() const {
			return moveSpeed;
		}

		inline void SetRotationSpeed(float rotation) {
			rotationSpeed = rotation;
		}
		inline float GetRotationSpeed() const {
			return rotationSpeed;
		}

		inline OrthographicCamera& GetCamera() {
			return camera;
		}
		inline const OrthographicCamera& GetCamera() const {
			return camera;
		}

		inline const OrthographicCameraBounds& GetBounds() const {
			return bounds;
		}

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		void CalculateView();
	};
}

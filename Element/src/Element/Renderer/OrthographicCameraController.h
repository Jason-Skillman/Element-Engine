#pragma once

#include "Element/Core/Timestep.h"

#include "Element/Events/ApplicationEvent.h"
#include "Element/Events/MouseEvent.h"

#include "Element/Renderer/OrthographicCamera.h"

namespace Element {
	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() const { return Right - Left; }
		float GetHeight() const { return Top - Bottom; }
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

		void OnResize(float width, float height);

		void SetZoomLevel(float zoomLevel) {
			this->zoomLevel = zoomLevel;
			CalculateView();
		}

		float GetZoomLevel() const {
			return zoomLevel;
		}

		void SetMoveSpeed(float speed) {
			moveSpeed = speed;
		}

		float GetMoveSpeed() const {
			return moveSpeed;
		}

		void SetRotationSpeed(float rotation) {
			rotationSpeed = rotation;
		}

		float GetRotationSpeed() const {
			return rotationSpeed;
		}

		OrthographicCamera& GetCamera() {
			return camera;
		}

		const OrthographicCamera& GetCamera() const {
			return camera;
		}

		const OrthographicCameraBounds& GetBounds() const {
			return bounds;
		}

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		void CalculateView();
	};
}

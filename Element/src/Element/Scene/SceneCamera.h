#pragma once

#include "Element/Renderer/Camera.h"

namespace Element {
	class SceneCamera : public Camera {
	public:
		enum class ProjectionType : uint8_t {
			Perspective = 0,
			Orthographic = 1
		};
		
	private:
		ProjectionType projectionType = ProjectionType::Orthographic;
		
		float perspectiveFOV = glm::radians(45.0f), perspectiveNear = 0.01f, perspectiveFar = 1000.0f;
		float orthographicSize = 10.0f, orthographicNear = -1.0f, orthographicFar = 1.0f;
		float aspectRatio;
		
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

	public:
		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const {
			return projectionType;
		}
		void SetProjectionType(ProjectionType projectionType) {
			this->projectionType = projectionType;
			RecalculateProjection();
		}

#pragma region Perspective
		void SetPerspective(float fov, float nearClip, float farClip);

		float GetPerspectiveFOV() const {
			return perspectiveFOV;
		}
		void SetPerspectiveSize(float size) {
			perspectiveFOV = size;
			RecalculateProjection();
		}

		float GetPerspectiveNearClip() const {
			return perspectiveNear;
		}
		void SetPerspectiveNearClip(float perspectiveNear) {
			this->perspectiveNear = perspectiveNear;
			RecalculateProjection();
		}

		float GetPerspectiveFarClip() const {
			return perspectiveFar;
		}
		void SetPerspectiveFarClip(float perspectiveFar) {
			this->perspectiveFar = perspectiveFar;
			RecalculateProjection();
		}
#pragma endregion

#pragma region Orthographic
		void SetOrthographic(float size, float nearClip, float farClip);

		float GetOrthographicSize() const {
			return orthographicSize;
		}
		void SetOrthographicSize(float size) {
			orthographicSize = size;
			RecalculateProjection();
		}

		float GetOrthographicNearClip() const {
			return orthographicNear;
		}
		void SetOrthographicNearClip(float orthographicNear) {
			this->orthographicNear = orthographicNear;
			RecalculateProjection();
		}

		float GetOrthographicFarClip() const {
			return orthographicFar;
		}
		void SetOrthographicFarClip(float orthographicFar) {
			this->orthographicFar = orthographicFar;
			RecalculateProjection();
		}
#pragma endregion
		
	private:
		void RecalculateProjection();
	};
}

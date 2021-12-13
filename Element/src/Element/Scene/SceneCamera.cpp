#include "pch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Element/Core/Core.h"

namespace Element {

	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		EL_CORE_ASSERT(width > 0 && height > 0, "Width or height cannot be less then or equal to 0!");

		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip) {
		projectionType = ProjectionType::Perspective;
		perspectiveFOV = fov;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		projectionType = ProjectionType::Orthographic;
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		if(projectionType == ProjectionType::Perspective) {
			projection = glm::perspective(perspectiveFOV, aspectRatio, perspectiveNear, perspectiveFar);
		} else {
			const float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
			const float orthoRight = orthographicSize * aspectRatio * 0.5f;
			const float orthoBottom = -orthographicSize * 0.5f;
			const float orthoTop = orthographicSize * 0.5f;

			projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
		}
	}
}

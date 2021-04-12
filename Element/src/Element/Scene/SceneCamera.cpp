#include "pch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Element {

	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		const float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
		const float orthoRight = orthographicSize * aspectRatio * 0.5f;
		const float orthoBottom = -orthographicSize * 0.5f;
		const float orthoTop = orthographicSize * 0.5f;

		projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
	}
}

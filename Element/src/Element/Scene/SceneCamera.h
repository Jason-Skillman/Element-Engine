#pragma once

#include "Element/Renderer/Camera.h"

namespace Element {
	class SceneCamera : public Camera {
	private:
		float orthographicSize = 10.0f, orthographicNear = -1.0f, orthographicFar = 1.0f;
		float aspectRatio;
		
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

	public:
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		inline float GetOrthographicSize() const {
			return orthographicSize;
		}
		inline void SetOrthographicSize(float size) {
			orthographicSize = size;
			RecalculateProjection();
		}

	private:
		void RecalculateProjection();
	};
}

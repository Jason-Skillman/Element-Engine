#pragma once

#include <glm/glm.hpp>

namespace Element {
	struct AspectRatio {
		float width;
		float height;

		static AspectRatio Ratio1x1;
		static AspectRatio Ratio16x9;
	};

	class OrthographicCamera {
	private:
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewProjectionMatrix;

		glm::vec3 position;
		float rotation = 0.0f;
		
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		OrthographicCamera(float scalar, AspectRatio ratio = AspectRatio::Ratio1x1);

	public:
		void SetProjection(float left, float right, float bottom, float top);

		const glm::mat4& GetProjectionMatrix() const {
			return projectionMatrix;
		}

		const glm::mat4& GetViewMatrix() const {
			return viewMatrix;
		}

		const glm::mat4& GetViewProjectionMatrix() const {
			return viewProjectionMatrix;
		}

		const glm::vec3& GetPosition() const {
			return position;
		}

		void SetPosition(const glm::vec3 position) {
			this->position = position;
			RecalculateViewMatrix();
		}

		float GetRotation() const {
			return rotation;
		}

		void SetRotation(float rotation) {
			this->rotation = rotation;
			RecalculateViewMatrix();
		}

	private:
		void RecalculateViewMatrix();
	};
}

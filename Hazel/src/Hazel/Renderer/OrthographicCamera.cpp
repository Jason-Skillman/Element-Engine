#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	AspectRatio AspectRatio::Ratio1x1 = { 1.0f, 1.0f };
	AspectRatio AspectRatio::Ratio16x9 = { 1.6f, 0.9f };

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), viewMatrix(1.0f), position(0.0f) {

		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
	
	OrthographicCamera::OrthographicCamera(float scalar, AspectRatio ratio)
		: projectionMatrix(glm::ortho(ratio.width * -scalar, ratio.width * scalar, ratio.height * -scalar, ratio.height* scalar, -1.0f, 1.0f)), viewMatrix(1.0f), position(0.0f) {

		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
		projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

		viewMatrix = glm::inverse(transform);

		//OpenGL: Proj * view * model * vertices
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
}

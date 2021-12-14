#include "pch.h"
#include "EditorCamera.h"

#include "Element/Core/Input.h"
#include "Element/Codes/KeyCodes.h"
#include "Element/Codes/MouseCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Element {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {

		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts) {
		EL_PROFILE_FUNCTION();

		//Maya style camera
		if(Input::IsKeyPressed(Key::LeftAlt)) {
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };

			glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
			initialMousePosition = mouse;

			if(Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if(Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if(Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.x);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EL_BIND_EVENT_FUNC(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	glm::vec3 EditorCamera::GetUpDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const {
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection() {
		aspectRatio = viewportWidth / viewportHeight;
		projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}

	glm::vec3 EditorCamera::CalculatePosition() const {
		return focalPoint - GetForwardDirection() * distance;
	}

	void EditorCamera::UpdateView() {
		//Lock the camera's rotation. Disables Alt + LMB for orthographic camera movement.
		//yaw = pitch = 0.0f;

		position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	void EditorCamera::MousePan(const glm::vec2& delta) {
		auto [xSpeed, ySpeed] = PanSpeed();
		focalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
		focalPoint += GetUpDirection() * delta.y * ySpeed * distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta) {
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * RotationSpeed();
		pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta) {
		distance -= delta * ZoomSpeed();
		if(distance < 1.0f) {
			focalPoint += GetForwardDirection();
			distance = 1.0f;
		}
	}

	std::pair<float, float> EditorCamera::PanSpeed() const {
		float x = std::min(viewportWidth / 1000.0f, 2.4f); //Max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewportHeight / 1000.0f, 2.4f); //Max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const {
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const {
		float dis = distance * 0.2f;
		dis = std::max(dis, 0.0f);
		float speed = dis * dis;
		speed = std::min(speed, 100.0f);	//Max speed = 100
		return speed;
	}
}

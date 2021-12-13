#include "pch.h"

#include <GLFW/glfw3.h>

#include "Element/Core/Input.h"
#include "Element/Core/Application.h"
#include "Element/Core/Window.h"

namespace Element {
	
	bool Input::IsKeyPressed(int keycode) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePosition();
		return x;
	}
	
	float Input::GetMouseY() {
		auto [x, y] = GetMousePosition();
		return y;
	}
}

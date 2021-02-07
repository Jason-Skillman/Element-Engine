#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowsWindow.h"

#include "Hazel/Log.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel {

	static bool glfwInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description)
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) : window(nullptr) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();

		context->SwapBuffers();
	}

	unsigned WindowsWindow::GetWidth() const {
		return data.width;
	}

	unsigned WindowsWindow::GetHeight() const {
		return data.height;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFunc& callback) {
		data.eventCallback = callback;
	}
	
	void WindowsWindow::SetVsync(bool enabled) {
		if(enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		data.vsync = enabled;
	}
	
	bool WindowsWindow::IsVsync() const {
		return data.vsync;
	}

	void WindowsWindow::Init(const WindowProps& props) {
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if(!glfwInitialized) {
			//Todo: glfwTerminate
			int success = glfwInit();
			HZ_CORE_ASSERT(!success, "Could not initalize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			glfwInitialized = true;
		}

		//Create window context
		window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), data.title.c_str(), nullptr, nullptr);

		context = new OpenGLContext(window);
		context->Init();

		glfwSetWindowUserPointer(window, &data);
		SetVsync(true);

		
		//Set GLFW callback events
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.width = width;
			data.height = height;

			//Trigger the event
			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			//Trigger the event
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			//Trigger the event
			switch(action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(character);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			//Trigger the event
			switch(action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			//Trigger the event
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			//Trigger the event
			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.eventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(window);
	}
}

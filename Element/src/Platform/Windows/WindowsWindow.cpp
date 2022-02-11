#include "pch.h"
#include "WindowsWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Element/Core/Log.h"
#include "Element/Events/ApplicationEvent.h"
#include "Element/Events/MouseEvent.h"
#include "Element/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Element {

	static bool glfwInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		EL_CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description)
	}

	Scope<Window> Window::Create(const WindowProps& props) {
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) : window(nullptr) {
		EL_PROFILE_FUNCTION();
		
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		EL_PROFILE_FUNCTION();

		Shutdown();
	}
	
	void WindowsWindow::OnUpdate() {
		EL_PROFILE_FUNCTION();
		
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

	void WindowsWindow::SetWindowIcon(const std::string& filepath) {
		if(filepath.empty()) return;

		GLFWimage images[1];
		images[0].pixels = stbi_load(filepath.c_str(), &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(images[0].pixels);
	}

	void WindowsWindow::Init(const WindowProps& props) {
		EL_PROFILE_FUNCTION();
		
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		EL_CORE_LOG_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if(!glfwInitialized) {
			//Todo: glfwTerminate
			int success = glfwInit();
			EL_CORE_ASSERT(success, "Could not initalize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			glfwInitialized = true;
		}

		//Hides the windows title bar
		//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

		//Create window context
		{
			EL_PROFILE_SCOPE("Create window");

			window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), data.title.c_str(), nullptr, nullptr);
		}

		context = new OpenGLContext(window);
		context->Init();

		glfwSetWindowUserPointer(window, &data);
		SetVsync(true);

		//Center the window in the middle of the main monitor
		{
			//Get the list of monitors
			/*int monitorCount;
			GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
			GLFWmonitor* monitor = monitors[0];
			if (monitors == nullptr) return;*/

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();

			//Get the monitor size from its video mode
			GLFWvidmode* monitorVidMode = (GLFWvidmode*)glfwGetVideoMode(monitor);

			int monitorWidth = monitorVidMode->width;
			int monitorHeight = monitorVidMode->height;

			glfwSetWindowPos(window, (monitorWidth - props.width) / 2, (monitorHeight - props.height) / 2);
		}

		{
			EL_PROFILE_SCOPE("Attach events");

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
	}

	void WindowsWindow::Shutdown() {
		EL_PROFILE_FUNCTION();
		
		glfwDestroyWindow(window);
	}
}

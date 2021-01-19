#include "hzpch.h"

#include <GLFW/glfw3.h>

#include "WindowsWindow.h"
#include "Hazel/Log.h"

namespace Hazel {

	static bool glfwInitialized = false;

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
		glfwSwapBuffers(window);
	}

	unsigned WindowsWindow::GetWidth() {
		return data.width;
	}

	unsigned WindowsWindow::GetHeight() {
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
			HZ_CORE_ASSERT(success, "Could not initalize GLFW!");

			glfwInitialized = true;
		}

		window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		SetVsync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(window);
	}
}

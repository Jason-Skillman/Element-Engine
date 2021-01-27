#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: windowHandle(window) {
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	
	void OpenGLContext::Init() {
		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initalized Glad!");
	}
	
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(windowHandle);
	}
}

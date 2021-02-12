#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Hazel/Core/Core.h"

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: windowHandle(window) {
		HZ_CORE_ASSERT(!windowHandle, "Window handle is null!");
	}
	
	void OpenGLContext::Init() {
		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(!status, "Failed to initalized Glad!");

		//Print graphics info
		HZ_CORE_INFO("OpenGL Renderer");
		HZ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(windowHandle);
	}
}

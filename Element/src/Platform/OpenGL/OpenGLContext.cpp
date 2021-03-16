#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Element/Core/Core.h"

namespace Element {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: windowHandle(window) {
		CORE_ASSERT(!windowHandle, "Window handle is null!");
	}
	
	void OpenGLContext::Init() {
		PROFILE_FUNCTION();
		
		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT(!status, "Failed to initalized Glad!");

		//Print graphics info
		LOG_CORE_INFO("OpenGL Renderer");
		LOG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers() {
		PROFILE_FUNCTION();
		
		glfwSwapBuffers(windowHandle);
	}
}

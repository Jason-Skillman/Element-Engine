#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Element/Core/Core.h"

namespace Element {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: windowHandle(window) {

		EL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	
	void OpenGLContext::Init() {
		EL_PROFILE_FUNCTION();
		
		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EL_CORE_ASSERT(status, "Failed to initalized Glad!");

		//Print graphics info
		EL_CORE_LOG_INFO("OpenGL Renderer");
		EL_CORE_LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		EL_CORE_LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		EL_CORE_LOG_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers() {
		EL_PROFILE_FUNCTION();
		
		glfwSwapBuffers(windowHandle);
	}
}

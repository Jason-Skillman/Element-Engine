#include "hzpch.h"

#include <GLFW/glfw3.h>

#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Hazel {
	Application::Application() {
		window = std::unique_ptr<Window>(Window::Create());
	}
	
	Application::~Application() = default;

	void Application::Run() {
		while(isRunning) {
			glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			window->OnUpdate();
		}
	}
}

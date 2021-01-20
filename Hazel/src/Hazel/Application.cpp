#include "hzpch.h"

#include <GLFW/glfw3.h>

#include "Application.h"
#include "Log.h"
#include "Events/Event.h"

namespace Hazel {

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)
	
	Application::Application() {
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));
	}
	
	Application::~Application() = default;

	void Application::Run() {
		while(isRunning) {
			glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		//HZ_CORE_TRACE("{0}", e)

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		isRunning = false;
		return true;
	}
}

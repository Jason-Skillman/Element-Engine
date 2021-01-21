#include "hzpch.h"

#include <glad/glad.h>

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

			for(Layer* layer : layerStack) {
				layer->OnUpdate();
			}
			
			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		//HZ_CORE_TRACE("{0}", e)

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));

		for(auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->OnEvent(e);
			if(e.IsHandled())
				break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		layerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		isRunning = false;
		return true;
	}
}

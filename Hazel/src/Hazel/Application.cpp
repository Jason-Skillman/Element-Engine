#include "hzpch.h"

#include <glad/glad.h>

#include "Hazel/Core.h"
#include "Application.h"

#include "Input.h"
#include "Log.h"
#include "Events/Event.h"

namespace Hazel {

	Application* Application::instance = nullptr;
	
	Application::Application() {
		if(!instance) instance = this;
		
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);
	}
	
	Application::~Application() = default;
	
	Application& Application::GetInstance() {
		if(!instance) instance = new Application();
		return *instance;
	}
	
	void Application::Run() {
		while(isRunning) {
			glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for(Layer* layer : layerStack) {
				layer->OnUpdate();
			}

			//Render each layer's ImGui
			imGuiLayer->Begin();
			for(Layer* layer : layerStack)
				layer->OnImGuiRender();
			imGuiLayer->End();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event) {
		//HZ_CORE_TRACE("{0}", event)

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));

		for(auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->OnEvent(event);
			if(event.IsHandled())
				break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Window& Application::GetWindow() const {
		return *window;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		isRunning = false;
		return true;
	}
}

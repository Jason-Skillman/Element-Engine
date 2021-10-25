#pragma once

#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

#include "Element/Events/ApplicationEvent.h"

#include "Element/ImGui/ImGuiLayer.h"

namespace Element {
	class Application {
	private:
		static Application* instance;
		
		Scope<Window> window;
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;
		
		bool isRunning = true, isMinimized = false;
		float lastFrameTime = 0.0f;
		
	public:
		Application(const std::string& name = std::string());
		virtual ~Application();

	public:
		static Application& GetInstance() {
			if(!instance) instance = new Application();
			return *instance;
		}
		
		void Run();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		/// <summary>
		/// Shuts down the application.
		/// </summary>
		void Close();

		ImGuiLayer* GetImGuiLayer() {
			return imGuiLayer;
		}

		Window& GetWindow() const {
			return *window;
		}

		bool IsMinimized() const {
			return isMinimized;
		}

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	};

	/// <summary>
	/// Defines the application for the engine to create.
	/// <para>Note: This method should be defined in the client.</para>
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}

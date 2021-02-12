#pragma once

#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {
	class Application {
	private:
		static Application* instance;
		
		std::unique_ptr<Window> window;
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;
		
		bool isRunning = true, isMinimized = false;
		float lastFrameTime = 0.0f;
		
	public:
		Application();
		virtual ~Application();

	public:
		inline static Application& GetInstance() {
			if(!instance) instance = new Application();
			return *instance;
		}
		
		void Run();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() const {
			return *window;
		}

		inline bool IsMinimized() const {
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

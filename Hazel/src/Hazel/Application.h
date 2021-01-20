#pragma once

#include "Core.h"
#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace Hazel {
	class HAZEL_API Application {
	private:
		std::unique_ptr<Window> window;
		bool isRunning = true;
		LayerStack layerStack;
		
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	/// <summary>
	/// Defines the application for the engine to create.
	/// <para>Note: This method should be defined in the client.</para>
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}

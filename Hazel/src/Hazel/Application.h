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
		static Application* instance;
		
		std::unique_ptr<Window> window;
		bool isRunning = true;
		LayerStack layerStack;
		
	public:
		Application();
		virtual ~Application();

	public:
		inline static Application& GetInstance();
		
		void Run();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() const;

	private:
		bool OnWindowClose(WindowCloseEvent& event);
	};

	/// <summary>
	/// Defines the application for the engine to create.
	/// <para>Note: This method should be defined in the client.</para>
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}

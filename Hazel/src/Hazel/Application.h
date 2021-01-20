#pragma once

#include "Core.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

//class WindowCloseEvent;

namespace Hazel {
	class HAZEL_API Application {
	private:
		std::unique_ptr<Window> window;
		bool isRunning = true;
		
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
		void OnEvent(Event& e);

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

#pragma once

#include "Core.h"
#include "Window.h"

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
	};

	/// <summary>
	/// Defines the application for the engine to create.
	/// <para>Note: This method should be defined in the client.</para>
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}

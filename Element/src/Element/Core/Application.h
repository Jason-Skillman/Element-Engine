#pragma once

#include "Core.h"
#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

#include "Element/Events/ApplicationEvent.h"
#include "Element/ImGui/ImGuiLayer.h"

namespace Element {

	struct ApplicationCommandLineArgs {
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const {
			EL_CORE_ASSERT(index < count, "Index cant be less than or equal to count!");
			return args[index];
		}
	};

	class Application {
	private:
		static Application* instance;

		ApplicationCommandLineArgs commandLineArgs;
		
		Scope<Window> window;
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;
		
		bool isRunning = true, isMinimized = false;
		float lastFrameTime = 0.0f;
		
	public:
		Application(const std::string& name = std::string(), ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
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

		ApplicationCommandLineArgs GetCommandLineArgs() const { 
			return commandLineArgs; 
		}

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	};

	/// <summary>
	/// Defines the application for the engine to create.
	/// <para>Note: This method should be defined in the client.</para>
	/// </summary>
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

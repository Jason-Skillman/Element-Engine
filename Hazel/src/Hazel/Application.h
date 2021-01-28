#pragma once

#include "Core.h"
#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"

namespace Hazel {
	class HAZEL_API Application {
	private:
		static Application* instance;
		
		std::unique_ptr<Window> window;
		ImGuiLayer* imGuiLayer;
		bool isRunning = true;
		LayerStack layerStack;

		unsigned int vertextArray, vertextBuffer, indexBuffer;
		std::unique_ptr<Shader> shader;
		
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

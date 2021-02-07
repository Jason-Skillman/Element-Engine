#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Application.h"

#include "Log.h"
#include "Events/Event.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

namespace Hazel {

	//Todo: Move OpenGL content
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch(type) {
			case ShaderDataType::Float:
				return GL_FLOAT;
			case ShaderDataType::Float2:
				return GL_FLOAT;
			case ShaderDataType::Float3:
				return GL_FLOAT;
			case ShaderDataType::Float4:
				return GL_FLOAT;
			case ShaderDataType::Mat3:
				return GL_FLOAT;
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			case ShaderDataType::Int:
				return GL_INT;
			case ShaderDataType::Int2:
				return GL_INT;
			case ShaderDataType::Int3:
				return GL_INT;
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::Bool:
				return GL_BOOL;
			default:
				HZ_CORE_ASSERT(true, "Unknown ShaderDataType: {0}", type);
				return 0;
		}
	}

	Application* Application::instance = nullptr;
	
	Application::Application() {
		if(!instance) instance = this;
		
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		Renderer::Init();

		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);
	}
	
	Application::~Application() = default;
	
	void Application::Run() {
		while(isRunning) {
			//Todo: Replace with Platform::GetTime()
			//Find delta time
			float time = static_cast<float>(glfwGetTime());
			Timestep timestep = time - lastFrameTime;;
			lastFrameTime = time;

			//Update each layer
			for(Layer* layer : layerStack) {
				layer->OnUpdate(timestep);
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

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		isRunning = false;
		return true;
	}
}

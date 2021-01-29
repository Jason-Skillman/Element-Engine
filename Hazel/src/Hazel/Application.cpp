#include "hzpch.h"

#include <glad/glad.h>

#include "Hazel/Core.h"
#include "Application.h"

#include "Input.h"
#include "Log.h"
#include "Events/Event.h"

namespace Hazel {

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
				HZ_CORE_ASSERT(false, "Unknown ShaderDataType: {0}", type);
				return 0;
		}
	}

	Application* Application::instance = nullptr;
	
	Application::Application() {
		if(!instance) instance = this;
		
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);

		
		
		//Todo: move
		glGenBuffers(1, &vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArray);

		
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->Bind();

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" }
		};

		uint32_t index = 0;
		for(const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type), 
				element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.offset));

			index++;
		}

		vertexBuffer->SetLayout(layout);
		
		

		unsigned int indices[3] = { 0, 1, 2 };
		unsigned int indicesSize = sizeof(indices) / sizeof(uint32_t);
		indexBuffer.reset(IndexBuffer::Create(indices, indicesSize));

		
		//Create shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 o_Color;

			in vec3 v_Position;

			void main() {
				o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
				//o_Color = vec4(0.8, 0.2, 0.3, 1.0);
			}
		)";

		shader.reset(new Shader(vertexSrc, fragmentSrc));
	}
	
	Application::~Application() = default;
	
	void Application::Run() {
		while(isRunning) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			//Todo: move
			shader->Bind();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			

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

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		isRunning = false;
		return true;
	}
}

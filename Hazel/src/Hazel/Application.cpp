#include "hzpch.h"

#include <glad/glad.h>

#include "Hazel/Core.h"
#include "Application.h"

#include "Input.h"
#include "Log.h"
#include "Events/Event.h"

//#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

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

		
		
		//Todo: Move
		vertexArray.reset(VertexArray::Create());
		
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->Bind();


		//Setup layout
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		//Layout must de set before
		vertexArray->AddVertexBuffer(vertexBuffer);
		
		

		unsigned int indices[3] = { 0, 1, 2 };
		unsigned int indicesSize = sizeof(indices) / sizeof(unsigned int);
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, indicesSize));

		vertexArray->SetIndexBuffer(indexBuffer);


		
		//Square
		//Todo: Move
		float sqVertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.8f, 0.0f, 1.0f, 1.0f
		};

		squareVA.reset(VertexArray::Create());
		//std::shared_ptr<VertexBuffer> squareVB = std::make_shared<VertexBuffer>(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		squareVB->SetLayout(layout);

		squareVA->AddVertexBuffer(squareVB);


		
		unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int squareIndicesSize = sizeof(squareIndices) / sizeof(unsigned int);
		
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, squareIndicesSize));

		squareVA->SetIndexBuffer(squareIB);



		
		//Create shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				gl_Position = vec4(a_Position, 1.0);
		
				v_Position = a_Position;
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 o_Color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				//o_Color = vec4(0.8, 0.2, 0.3, 1.0);
				//o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
				o_Color = v_Color;
			}
		)";

		shader.reset(new Shader(vertexSrc, fragmentSrc));
	}
	
	Application::~Application() = default;
	
	void Application::Run() {
		while(isRunning) {
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			
			//Start rendering
			Renderer::BeginScene();

			shader->Bind();
			Renderer::Submit(squareVA);
			//Renderer::Submit(vertexArray);

			Renderer::EndScene();
			

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

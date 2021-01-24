#include "hzpch.h"

#include <GLFW/glfw3.h>

//Todo: remove
#include <glad/glad.h>

#include "Hazel/Core.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Hazel/Application.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {}
	
    ImGuiLayer::~ImGuiLayer() = default;
	
	void ImGuiLayer::OnAttach() {
        //----- Setup ImGui -----
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//Todo: Use custom key codes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
	}
	
	void ImGuiLayer::OnDetach() {}

	void ImGuiLayer::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetInstance();
        io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

        float time = (float)glfwGetTime();
        io.DeltaTime = deltaTime > 0.0f ? (time - deltaTime) : (1.0f / 60.0f);
        deltaTime = time;

        //ImGui New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
		
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
		
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	void ImGuiLayer::OnEvent(Event& event) {
		//HZ_PRINT_TRACE("{0}", event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(ImGuiLayer::OnWindowResizeEvent));
	}

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;

        return false;
	}
	
    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = false;

        return false;
	}
	
    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());
		
        return false;
	}
	
    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += event.GetYOffset();
        io.MouseWheelH += event.GetXOffset();

        return false;
	}
	
    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = true;
		
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		
        return false;
	}
	
    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = false;
		
        return false;
	}

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
		
        if(event.GetKeyCode() > 0 && event.GetKeyCode() < 0x10000)
            io.AddInputCharacter(static_cast<unsigned short>(event.GetKeyCode()));

        return false;
    }
	
    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());
		
        return false;
	}
}

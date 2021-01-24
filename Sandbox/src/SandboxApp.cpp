#include <Hazel.h>

#include "ExampleLayer.h"

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}
	~Sandbox() {
		
	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}

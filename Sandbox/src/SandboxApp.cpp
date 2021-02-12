#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Layers/ExampleLayer.h"
#include "Layers/SandboxLayer.h"

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new SandboxLayer());
	}
	~Sandbox() = default;
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}

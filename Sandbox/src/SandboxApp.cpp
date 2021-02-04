#include <Hazel.h>

#include "ExampleLayer.h"

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() = default;
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}

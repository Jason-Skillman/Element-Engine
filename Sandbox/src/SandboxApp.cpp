#include <Element.h>
#include <Element/Core/EntryPoint.h>

#include "Layers/ExampleLayer.h"
#include "Layers/SandboxLayer.h"

class Sandbox : public Element::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new SandboxLayer());
	}
	~Sandbox() = default;
};

Element::Application* Element::CreateApplication() {
	return new Sandbox();
}

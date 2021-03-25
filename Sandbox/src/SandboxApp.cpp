#include <Element.h>
#include <Element/Core/EntryPoint.h>

#include "Layers/ExampleLayer.h"
#include "Layers/SpriteSheetLayer.h"
#include "Layers/Renderer2DLayer.h"
#include "Layers/SandboxLayer.h"

class Sandbox : public Element::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		//PushLayer(new SpriteSheetLayer());
		//PushLayer(new Renderer2DLayer());
		PushLayer(new SandboxLayer());
	}
	~Sandbox() = default;
};

Element::Application* Element::CreateApplication() {
	return new Sandbox();
}

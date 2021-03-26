#include <Element.h>
#include <Element/Core/EntryPoint.h>

#include "Layers/ExampleLayer.h"
#include "Layers/SpriteSheetLayer.h"
#include "Layers/Renderer2DLayer.h"
#include "Layers/SandboxLayer.h"

class ElementEditorApp : public Element::Application {
public:
	ElementEditorApp() {
		//PushLayer(new ExampleLayer());
		//PushLayer(new SpriteSheetLayer());
		//PushLayer(new Renderer2DLayer());
		PushLayer(new SandboxLayer());
	}
	~ElementEditorApp() = default;
};

Element::Application* Element::CreateApplication() {
	return new ElementEditorApp();
}

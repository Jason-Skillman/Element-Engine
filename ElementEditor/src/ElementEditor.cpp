#include <Element/Core/EntryPoint.h>

#include "Layers/EditorLayer.h"

namespace Element {

	class ElementEditor : public Application {
	public:
		ElementEditor(ApplicationCommandLineArgs args)
			: Application("Element Editor", "Resources/element_engine.png", args) {
			
			PushLayer(new EditorLayer());
		}
		~ElementEditor() = default;
	};

	Application* Element::CreateApplication(ApplicationCommandLineArgs args) {
		return new ElementEditor(args);
	}
}

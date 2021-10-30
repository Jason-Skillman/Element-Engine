#include <Element/Core/EntryPoint.h>

#include "Layers/EditorLayer.h"

namespace Element {

	class ElementEditor : public Application {
	public:
		ElementEditor()
			: Application("Element Editor") {
			
			PushLayer(new EditorLayer());
		}
		~ElementEditor() = default;
	};

	Application* Element::CreateApplication() {
		return new ElementEditor();
	}
}

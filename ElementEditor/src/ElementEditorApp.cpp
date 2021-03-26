#include <Element/Core/EntryPoint.h>

#include "Layers/EditorLayer.h"

namespace Element {
	class ElementEditorApp : public Application {
	public:
		ElementEditorApp() {
			PushLayer(new EditorLayer());
		}
		~ElementEditorApp() = default;
	};

	Application* Element::CreateApplication() {
		return new ElementEditorApp();
	}
}

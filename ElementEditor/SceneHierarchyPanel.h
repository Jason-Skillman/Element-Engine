#pragma once

#include "Element/Core/Core.h"
#include "Element/Scene/Scene.h"

namespace Element {
	class SceneHierarchyPanel {
	private:
		Ref<Scene> context;
		
	public:
		SceneHierarchyPanel(const Ref<Scene>& context);

	public:
		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	};
}

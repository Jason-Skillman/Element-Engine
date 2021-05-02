#pragma once

#include "Element.h"
#include "Element/Core/Core.h"
#include "Element/Core/Log.h"
#include "Element/Scene/Scene.h"
#include "Element/Scene/Entity.h"

namespace Element {
	class SceneHierarchyPanel {
	private:
		Ref<Scene> context;
		Entity selectionContext;
		
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

	public:
		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
	};
}

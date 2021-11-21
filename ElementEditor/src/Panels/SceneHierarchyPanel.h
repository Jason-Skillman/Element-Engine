#pragma once

#include "Element.h"

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

		Entity GetSelectedEntity() const {
			return selectionContext;
		}
		void SetSelectedEntity(Entity entity) {
			selectionContext = entity;
		}

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	};
}

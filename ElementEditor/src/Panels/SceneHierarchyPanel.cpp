#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "Element/Scene/Components.h"

namespace Element {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}
	
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		this->context = context;
	}
	
	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");

		context->registry.each([&](auto entityId) {
			Entity entity{ entityId, context.get() };
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
		
		auto& tag = entity.GetComponent<TagComponent>().tag;

		const ImGuiTreeNodeFlags flags = selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint32_t>(entity)), flags, tag.c_str());

		if(ImGui::IsItemClicked())
			selectionContext = entity;

		if(opened)
			ImGui::TreePop();

	}
}

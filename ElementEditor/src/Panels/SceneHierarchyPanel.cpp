#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

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

		//Removes the selected context when clicking in a blank area
		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");

		if(selectionContext) {
			DrawComponents(selectionContext);
		}
		
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

	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		//Draws the name
		if(entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			
			if(ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		//Draws the transform
		if(entity.HasComponent<TransformComponent>()) {
			if(ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}
	}
}

#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "ImGui/ImGuiEditorUtils.h"
#include "Element/Scene/Components.h"

namespace Element {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}
	
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		this->context = context;
	}
	
	void SceneHierarchyPanel::OnImGuiRender() {
		//Draw the scene hierarchy window
		{
			ImGui::Begin("Scene Hierarchy");

			context->registry.each([&](auto entityId) {
				Entity entity{ entityId, context.get() };
				DrawEntityNode(entity);
				});

			//Removes the selected context when clicking in a blank area
			if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				selectionContext = {};

			//Context menu for the window (blank space)
			if(ImGui::BeginPopupContextWindow(0, 1, false)) {
				if(ImGui::MenuItem("Create Empty Entity"))
					context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}

			ImGui::End();
		}

		//Draw the properties window
		{
			ImGui::Begin("Properties");

			if(selectionContext) {
				DrawComponents(selectionContext);
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().tag;

		const ImGuiTreeNodeFlags flags = (selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint32_t>(entity)), flags, tag.c_str());

		if(ImGui::IsItemClicked())
			selectionContext = entity;

		//Context menu
		bool deleteEntity = false;
		if(ImGui::BeginPopupContextItem()) {
			if(ImGui::MenuItem("Delete"))
				deleteEntity = true;

			ImGui::EndPopup();
		}

		if(opened)
			ImGui::TreePop();

		if(deleteEntity) {
			context->DestroyEntity(entity);
			if(selectionContext == entity)
				selectionContext = {};
		}
			
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
				auto& transformComponent = entity.GetComponent<TransformComponent>();

				//ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.translation));

				Editor::DrawVec3Control("Translation", transformComponent.translation);

				glm::vec3 rotationDegrees = glm::degrees(transformComponent.rotation);
				Editor::DrawVec3Control("Rotation", rotationDegrees);
				transformComponent.rotation = glm::radians(rotationDegrees);
				
				Editor::DrawVec3Control("Scale", transformComponent.scale, 1.0f);

				ImGui::TreePop();
			}
		}

		//Draws the camera
		if(entity.HasComponent<CameraComponent>()) {
			if(ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				ImGui::Checkbox("Primary", &cameraComponent.primary);

				const char* projectionTypes[] = { "Perspective", "Orthographic" };
				const char* currentProjection = projectionTypes[static_cast<int>(camera.GetProjectionType())];
				
				if(ImGui::BeginCombo("Projection", currentProjection, ImGuiTreeNodeFlags_DefaultOpen)) {
					//Populate combo list
					for(int i = 0; i < 2; i++) {
						bool isSelected = currentProjection == projectionTypes[i];

						if(ImGui::Selectable(projectionTypes[i], isSelected)) {
							currentProjection = projectionTypes[i];

							camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if(isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Draw perspective properties
				if(camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
					if(ImGui::DragFloat("Vertical FOV", &perspectiveFOV))
						camera.SetPerspectiveSize(glm::radians(perspectiveFOV));

					float PerspectiveNearClip = camera.GetPerspectiveNearClip();
					if(ImGui::DragFloat("Near Clip", &PerspectiveNearClip))
						camera.SetPerspectiveNearClip(PerspectiveNearClip);

					float PerspectiveFarClip = camera.GetPerspectiveFarClip();
					if(ImGui::DragFloat("Far Clip", &PerspectiveFarClip))
						camera.SetPerspectiveFarClip(PerspectiveFarClip);
				}
				//Draw orthographic properties
				else if(camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
					
					float orthographicSize = camera.GetOrthographicSize();
					if(ImGui::DragFloat("Size", &orthographicSize))
						camera.SetOrthographicSize(orthographicSize);

					float orthographicNearClip = camera.GetOrthographicNearClip();
					if(ImGui::DragFloat("Near Clip", &orthographicNearClip))
						camera.SetOrthographicNearClip(orthographicNearClip);

					float orthographicFarClip = camera.GetOrthographicFarClip();
					if(ImGui::DragFloat("Far Clip", &orthographicFarClip))
						camera.SetOrthographicFarClip(orthographicFarClip);
				}
				
				ImGui::TreePop();
			}
		}

		//Draws sprite renderer
		if(entity.HasComponent<SpriteRendererComponent>()) {
			if(ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
				auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.color));

				ImGui::TreePop();
			}
		}
	}
}

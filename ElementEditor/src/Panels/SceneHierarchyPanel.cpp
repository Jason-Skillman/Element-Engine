#include "SceneHierarchyPanel.h"

#include <cstring>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/ImGuiEditorUtils.h"

namespace Element {

	//Todo: Change directory when project locations are added
	//Defined in "ContentBrowserPanel.cpp"
	extern const std::filesystem::path assetsPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}
	
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		this->context = context;
		selectionContext = {};
	}
	
	void SceneHierarchyPanel::OnImGuiRender() {
		//Draw the scene hierarchy window
		{
			ImGui::Begin("Scene Hierarchy");

			if(context) {
				context->registry.each([&](auto entityID) {
					Entity entity{ entityID, context.get() };
					DrawEntityNode(entity);
				});

				//Removes the selected context when clicking in a blank area
				if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					selectionContext = {};

				//Context menu for the window (blank space)
				if(ImGui::BeginPopupContextWindow(0, 1, false)) {

					if(ImGui::MenuItem("Create Entity")) {
						Entity newEntity = context->CreateEntity("Entity");
						selectionContext = newEntity;
					} else if(ImGui::MenuItem("Sprite Renderer")) {
						Entity newEntity = context->CreateEntity("Sprite Renderer");
						newEntity.AddComponent<SpriteRendererComponent>();
						selectionContext = newEntity;
					} else if(ImGui::MenuItem("Camera")) {
						Entity newEntity = context->CreateEntity("Camera");
						newEntity.AddComponent<CameraComponent>();
						selectionContext = newEntity;
					}

					ImGui::EndPopup();
				}
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
		std::string& tag = entity.GetComponent<TagComponent>().tag;

		const ImGuiTreeNodeFlags flags = (selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction function, bool canRemove = true) {
		if(entity.HasComponent<T>()) {
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

			T& component = entity.GetComponent<T>();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			//Settings button
			bool removeComponent = false;
			{
				ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
				const char* settingID = "ComponentSettings";
				if(ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
					ImGui::OpenPopup(settingID);
				}
				//ImGui::PopStyleVar();


				if(ImGui::BeginPopup(settingID)) {
					if(canRemove)
						if(ImGui::MenuItem("Remove Component"))
							removeComponent = true;

					ImGui::EndPopup();
				}
			}

			if(open) {
				function(component);

				ImGui::TreePop();
			}

			if(canRemove && removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	/// <summary>Draws all elements and components for the entity in the properties panel.</summary>
	/// <param name="entity">The current entity.</param>
	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		//Draw the name of the entity
		if(entity.HasComponent<TagComponent>()) {
			std::string& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			
			if(ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		//Draw the add component button
		{
			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			const char* addID = "AddComponent";

			if(ImGui::Button("Add Component"))
				ImGui::OpenPopup(addID);

			if(ImGui::BeginPopup(addID)) {

				if(!selectionContext.HasComponent<CameraComponent>())
					if(ImGui::MenuItem("Camera")) {
						selectionContext.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}

				if(!selectionContext.HasComponent<SpriteRendererComponent>())
					if(ImGui::MenuItem("Sprite Renderer")) {
						selectionContext.AddComponent<SpriteRendererComponent>();
						ImGui::CloseCurrentPopup();
					}

				if(!selectionContext.HasComponent<Rigidbody2DComponent>())
					if(ImGui::MenuItem("Rigidbody 2D")) {
						selectionContext.AddComponent<Rigidbody2DComponent>();
						ImGui::CloseCurrentPopup();
					}

				if(!selectionContext.HasComponent<BoxCollider2DComponent>())
					if(ImGui::MenuItem("Box Collider 2D")) {
						selectionContext.AddComponent<BoxCollider2DComponent>();
						ImGui::CloseCurrentPopup();
					}

				ImGui::EndPopup();
			}

			ImGui::PopItemWidth;
		}

		//Transform component
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			Editor::DrawVec3Control("Translation", component.translation);

			//Convert radians to degrees then save back as radians
			glm::vec3 rotationDegrees = glm::degrees(component.rotation);
			Editor::DrawVec3Control("Rotation", rotationDegrees);
			component.rotation = glm::radians(rotationDegrees);

			Editor::DrawVec3Control("Scale", component.scale, 1.0f);
		}, false);

		//Camera component
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.camera;

			ImGui::Checkbox("Primary", &component.primary);

			const char* projectionTypes[] = { "Perspective", "Orthographic" };
			const char* currentProjection = projectionTypes[static_cast<int>(camera.GetProjectionType())];

			if(ImGui::BeginCombo("Projection", currentProjection, ImGuiTreeNodeFlags_DefaultOpen)) {
				//Populate combo list
				for(int i = 0; i < 2; i++) {
					bool isSelected = (currentProjection == projectionTypes[i]);

					if(ImGui::Selectable(projectionTypes[i], isSelected)) {
						currentProjection = projectionTypes[i];

						camera.SetProjectionType(static_cast<ProjectionType>(i));
					}

					if(isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			//Draw perspective properties
			if(camera.GetProjectionType() == ProjectionType::Perspective) {
				float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
				if(ImGui::DragFloat("Vertical FOV", &perspectiveFOV))
					camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));

				float PerspectiveNearClip = camera.GetPerspectiveNearClip();
				if(ImGui::DragFloat("Near Clip", &PerspectiveNearClip))
					camera.SetPerspectiveNearClip(PerspectiveNearClip);

				float PerspectiveFarClip = camera.GetPerspectiveFarClip();
				if(ImGui::DragFloat("Far Clip", &PerspectiveFarClip))
					camera.SetPerspectiveFarClip(PerspectiveFarClip);
			}
			//Draw orthographic properties
			else if(camera.GetProjectionType() == ProjectionType::Orthographic) {
				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);

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
		});

		//Sprite renderer component
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

			//Texture field
			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

			//Setup drag drop events
			if(ImGui::BeginDragDropTarget()) {
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = static_cast<const wchar_t*>(payload->Data);
					std::filesystem::path texturePath = std::filesystem::path(assetsPath) / path;
					component.texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling", &component.tiling, 0.1f, 0.0f, 100.0f);
		});

		//Rigidbody 2D component
		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component) {
			const int8_t bodyTypeCount = 3;
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic"};
			const char* currentBodyTypeString = bodyTypeStrings[static_cast<int>(component.bodyType)];

			if(ImGui::BeginCombo("Body Type", currentBodyTypeString, ImGuiTreeNodeFlags_DefaultOpen)) {
				//Populate combo list
				for(int i = 0; i < bodyTypeCount; i++) {
					bool isSelected = (currentBodyTypeString == bodyTypeStrings[i]);
					if(ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
						currentBodyTypeString = bodyTypeStrings[i];

						component.bodyType = static_cast<Rigidbody2DComponent::BodyType>(i);
					}

					if(isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed rotation", &component.fixedRotation);
		});

		//Box collider 2D component
		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component) {
			const float dragSpeed = 0.01f;

			ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.size));
			ImGui::DragFloat("Densisty", &component.density, dragSpeed, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.friction, dragSpeed, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.restitution, dragSpeed, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, dragSpeed, 0.0f);
		});
	}
}

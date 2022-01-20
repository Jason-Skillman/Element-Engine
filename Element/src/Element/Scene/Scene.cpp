#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "Components.h"
#include "Entity.h"
#include "Element/Renderer/Renderer2D.h"

namespace Element {

	Scene::Scene() {}
	
	Scene::~Scene() {}

	void Scene::OnUpdateRuntime(Timestep ts) {
		EL_PROFILE_FUNCTION();

		//Update script functions
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if(!nsc.instance) {
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->entity = Entity{ entity, this };
					nsc.instance->OnCreate();
				}

				nsc.instance->OnUpdate(ts);
			});
		}

		//Setup camera
		Camera* mainCamera = nullptr;
		glm::mat4 mainCameraTransform;
		{
			auto view = registry.view<TransformComponent, CameraComponent>();
			for(auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if(camera.primary) {
					mainCamera = &camera.camera;
					mainCameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		//Begin the scene and draw all objects
		if(mainCamera) {
			Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

			//Render all sprite components
			auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		EL_PROFILE_FUNCTION();

		Renderer2D::BeginScene(camera);

		//Render all sprite components
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for(auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		viewportWidth = width;
		viewportHeight = height;

		auto view = registry.view<CameraComponent>();
		for(auto entity : view) {
			auto& camera = view.get<CameraComponent>(entity);

			if(!camera.fixedAspectRatio)
				camera.camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = registry.view<CameraComponent>();
		for(auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if(camera.primary)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::CreateEntity(const std::string& name) {
		EL_PROFILE_FUNCTION();

		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag = !name.empty() ? name : std::string("Entity");
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		EL_PROFILE_FUNCTION();

		registry.destroy(entity);
	}

	#pragma region OnComponentAdded<>() Definitions

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		//We dont support generic types. Instead define below.
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if(viewportWidth > 0 && viewportHeight > 0)
			component.camera.SetViewportSize(viewportWidth, viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	#pragma endregion
}

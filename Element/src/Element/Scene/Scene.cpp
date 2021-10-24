#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "Components.h"
#include "Entity.h"
#include "Element/Renderer/Renderer2D.h"

namespace Element {

	Scene::Scene() {}
	
	Scene::~Scene() {}

	void Scene::OnUpdate(Timestep ts) {

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
			
			auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawPropertiesMat4 drawProps;
				drawProps.transform = transform.GetTransform();
				drawProps.color = sprite.color;
				Renderer2D::DrawQuad(drawProps);
			}

			Renderer2D::EndScene();
		}
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

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		
		auto& tag = entity.AddComponent<TagComponent>();
		tag = !name.empty() ? name : std::string("Entity");
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		registry.destroy(entity);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component) {}

	template<>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component) {
		component.camera.SetViewportSize(viewportWidth, viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, SpriteRendererComponent& component) {}

	template<>
	void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component) {}
}

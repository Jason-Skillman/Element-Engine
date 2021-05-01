#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.inl>

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
		glm::mat4* mainCameraTransform = nullptr;
		{
			auto view = registry.view<TransformComponent, CameraComponent>();
			for(auto entity : view) {
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if(camera.primary) {
					mainCamera = &camera.camera;
					mainCameraTransform = &transform.transform;
					break;
				}
			}
		}

		//Begin the scene and draw all objects
		if(mainCamera) {
			Renderer2D::BeginScene(*mainCamera, *mainCameraTransform);
			
			auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawPropertiesMat4 drawProps;
				drawProps.transform = transform;
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
		entity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), { 4.0f, 4.0f, 0.0f }));
		
		auto& tag = entity.AddComponent<TagComponent>();
		tag = !name.empty() ? name : std::string("Entity");
		
		return entity;
	}
}

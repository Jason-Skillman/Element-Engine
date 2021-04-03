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
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for(auto& entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawPropertiesMat4 drawProps;
			drawProps.transform = transform;
			drawProps.color = sprite.color;
			Renderer2D::DrawQuad(drawProps);
		}
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		
		auto& tag = entity.AddComponent<TagComponent>();
		tag = !name.empty() ? name : std::string("Entity");
		
		return entity;
	}
}

#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.inl>

#include "Components.h"
#include "Element/Renderer/Renderer2D.h"

namespace Element {

	Scene::Scene() {
		
		entt::entity entity = registry.create();

		registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	}
	
	Scene::~Scene() {
		
	}

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

	entt::entity Scene::CreateEntity() {
		return registry.create();
	}
}

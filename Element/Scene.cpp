#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace Element {

	Scene::Scene() {
		struct TransformComponent {
			glm::mat4 transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: transform(transform) {}

			operator glm::mat4& () {
				return transform;
			}
			operator const glm::mat4& () const {
				return transform;
			}
		};

		entt::entity entity = registry.create();

		registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	}
	
	Scene::~Scene() {
		
	}
}

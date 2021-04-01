#pragma once

#include "glm/glm.hpp"

namespace Element {

	struct TransformComponent {
		glm::mat4 transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 & transform)
			: transform(transform) {}

		operator glm::mat4& () {
			return transform;
		}
		operator const glm::mat4& () const {
			return transform;
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 color = glm::vec4(0.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
	};
}

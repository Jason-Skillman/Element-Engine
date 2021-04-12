#pragma once

#include <glm/glm.hpp>


#include "Element/Scene/SceneCamera.h"

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

	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}

		operator std::string& () {
			return tag;
		}
		operator const std::string& () const {
			return tag;
		}
	};

	struct CameraComponent {
		SceneCamera camera;
		bool primary = true;	//Todo: Move to scene
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}

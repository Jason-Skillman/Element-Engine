#pragma once

#include "glm/glm.hpp"

namespace Element {
	class Camera {
	protected:
		glm::mat4 projection = glm::mat4(1.0f);
		
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: projection(projection) {}
		virtual ~Camera() = default;

	public:
		const glm::mat4& GetProjection() const {
			return projection;
		}		
	};
}

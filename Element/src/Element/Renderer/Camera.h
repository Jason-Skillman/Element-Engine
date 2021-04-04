#pragma once

#include "glm/glm.hpp"

namespace Element {
	class Camera {
	public:
		Camera(const glm::mat4& projecttion)
			: projection(projecttion) {}

		inline const glm::mat4& GetProjecttion() const {
			return projection;
		}
		
	private:
		glm::mat4 projection;
		
	};
}
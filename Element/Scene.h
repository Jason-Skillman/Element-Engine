#pragma once

#include "entt.hpp"

namespace Element {
	class Scene {
	private:
		entt::registry registry;
		
	public:
		Scene();
		~Scene();
	};
}

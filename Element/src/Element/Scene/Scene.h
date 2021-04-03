#pragma once

#include "entt.hpp"

#include "Element/Core/Timestep.h"

namespace Element {
	
	class Entity;

	class Scene {
	private:
		entt::registry registry;

		friend class Entity;
		
	public:
		Scene();
		~Scene();

	public:
		void OnUpdate(Timestep ts);

		Entity CreateEntity(const std::string& name = std::string());
	};
}

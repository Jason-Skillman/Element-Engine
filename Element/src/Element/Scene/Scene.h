#pragma once

#include "entt.hpp"

#include "Element/Core/Timestep.h"

namespace Element {
	class Entity;

	class Scene {
	private:
		entt::registry registry;
		uint32_t viewportWidth = 0, viewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		
	public:
		Scene();
		~Scene();

	public:
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = std::string());
	};
}

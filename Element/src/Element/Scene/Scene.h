#pragma once

#include <entt.hpp>

#include "Element/Core/Timestep.h"
#include "Element/Renderer/EditorCamera.h"

namespace Element {

	class Entity;

	class Scene {
	private:
		entt::registry registry;
		uint32_t viewportWidth = 0, viewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		
	public:
		Scene();
		~Scene();

	public:
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity GetPrimaryCameraEntity();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	};
}

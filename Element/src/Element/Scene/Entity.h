#pragma once

#include "entt.hpp"

#include "Element/Core/Core.h"
#include "Scene.h"

namespace Element {
	class Entity {
	private:
		entt::entity entityId = entt::null;
		Scene* scene = nullptr;

	public:
		Entity() = default;
		Entity(entt::entity entityId, Scene* scene);
		Entity(const Entity& other) = default;

	public:
		template<typename T, typename... Args>
		inline T& AddComponent(Args&&... args) {
			EL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component: " + std::string(typeid(T).name()));
			
			return scene->registry.emplace<T>(entityId, std::forward<Args>(args)...);
		}

		template<typename T>
		inline T& GetComponent() {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + std::string(typeid(T).name()));
			
			return scene->registry.get<T>(entityId);
		}

		template<typename T>
		inline void RemoveComponent() {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + std::string(typeid(T).name()));
			
			scene->registry.remove<T>(entityId);
		}
		
		template<typename T>
		inline bool HasComponent() {
			return scene->registry.has<T>(entityId);
		}

		operator bool() const {
			return entityId != entt::null;
		}

	};
}

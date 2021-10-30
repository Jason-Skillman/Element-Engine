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
		T& AddComponent(Args&&... args) {
			EL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component: " + std::string(typeid(T).name()));

			T& component = scene->registry.emplace<T>(entityId, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() const {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + std::string(typeid(T).name()));
			
			return scene->registry.get<T>(entityId);
		}

		template<typename T>
		void RemoveComponent() {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + std::string(typeid(T).name()));
			
			scene->registry.remove<T>(entityId);
		}
		
		template<typename T>
		bool HasComponent() const {
			return scene->registry.has<T>(entityId);
		}

		operator bool() const {
			return entityId != entt::null;
		}

		operator uint32_t() const {
			return static_cast<uint32_t>(entityId);
		}
		
		operator entt::entity() const {
			return entityId;
		}

		bool operator ==(const Entity& other) const {
			return entityId == other.entityId && scene == other.scene;
		}
		
		bool operator !=(const Entity& other) const {
			return !(*this == other);
		}
	};
}

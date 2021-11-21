#pragma once

#include "entt.hpp"

#include "Element/Core/Core.h"
#include "Scene.h"

namespace Element {

	class Entity {
	private:
		entt::entity entityID = entt::null;
		Scene* scene = nullptr;

	public:
		Entity() = default;
		Entity(entt::entity entityID, Scene* scene);
		Entity(const Entity& other) = default;

	public:
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			EL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component: " + std::string(typeid(T).name()));

			T& component = scene->registry.emplace<T>(entityID, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() const {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + std::string(typeid(T).name()));
			
			return scene->registry.get<T>(entityID);
		}

		template<typename T>
		void RemoveComponent() {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + std::string(typeid(T).name()));
			
			scene->registry.remove<T>(entityID);
		}
		
		template<typename T>
		bool HasComponent() const {
			return scene->registry.has<T>(entityID);
		}

		operator bool() const {
			return entityID != entt::null;
		}

		operator uint32_t() const {
			return static_cast<uint32_t>(entityID);
		}
		
		operator entt::entity() const {
			return entityID;
		}

		bool operator ==(const Entity& other) const {
			return entityID == other.entityID && scene == other.scene;
		}
		
		bool operator !=(const Entity& other) const {
			return !(*this == other);
		}
	};
}

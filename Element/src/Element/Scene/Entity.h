#pragma once

#include "entt.hpp"

#include "Element/Core/Core.h"
#include "Element/Core/GUID.h"
#include "Scene.h"
#include "Components.h"

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
			EL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component: {0}", std::string(typeid(T).name()));

			T& component = scene->registry.emplace<T>(entityID, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args) {
			T& component = scene->registry.emplace_or_replace<T>(entityID, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() const {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: {0}", std::string(typeid(T).name()));
			
			return scene->registry.get<T>(entityID);
		}

		template<typename T, typename... Args>
		bool TryAddComponent(Args&&... args) {
			if(HasComponent<T>()) return false;
			
			T& component = scene->registry.emplace<T>(entityID, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return true;
		}

		template<typename T>
		bool TryGetComponent(T& comp) const {
			if(!HasComponent<T>()) return false;

			comp = scene->registry.get<T>(entityID);
			return true;
		}

		template<typename T>
		void RemoveComponent() {
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: {0}", std::string(typeid(T).name()));
			
			scene->registry.remove<T>(entityID);
		}
		
		template<typename T>
		bool HasComponent() const {
			return scene->registry.has<T>(entityID);
		}

		GUID GetGUID() {
			return GetComponent<IDComponent>().guid;
		}

		const std::string& GetName() {
			return GetComponent<TagComponent>().tag;
		}

	public:
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

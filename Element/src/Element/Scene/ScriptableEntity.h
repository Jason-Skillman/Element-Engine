#pragma once

#include "Entity.h"

namespace Element {

	class ScriptableEntity {
	private:
		Entity entity;

		friend class Scene;

	public:
		virtual ~ScriptableEntity() = default;
 
	public:
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return entity.AddComponent<T>(std::forward<Args>(args)...);
		}
		
		template<typename T>
		T& GetComponent() const {
			return entity.GetComponent<T>();
		}

		template<typename T>
		void RemoveComponent() {
			entity.RemoveComponent<T>();
		}

		template<typename T>
		bool HasComponent() const {
			return entity.HasComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	};
}

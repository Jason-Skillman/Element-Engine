#pragma once

#include "Entity.h"

namespace Element 	{
	class ScriptableEntity {
	private:
		Entity entity;

		friend class Scene;

	public:
		template<typename T, typename... Args>
		inline T& AddComponent(Args&&... args) {
			return entity.AddComponent<T>(std::forward<Args>(args)...);
		}
		
		template<typename T>
		inline T& GetComponent() const {
			return entity.GetComponent<T>();
		}

		template<typename T>
		inline void RemoveComponent() {
			entity.RemoveComponent<T>();
		}

		template<typename T>
		inline bool HasComponent() const {
			return entity.HasComponent<T>();
		}
	};
}

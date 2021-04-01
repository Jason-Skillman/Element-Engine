#pragma once

#include "entt.hpp"

#include "Element/Core/Timestep.h"

namespace Element {
	class Scene {
	private:
		entt::registry registry;
		
	public:
		Scene();
		~Scene();

	public:
		void OnUpdate(Timestep ts);

		entt::entity CreateEntity();

		//Todo: Temp
		entt::registry& Reg() {
			return registry;
		}
	};
}

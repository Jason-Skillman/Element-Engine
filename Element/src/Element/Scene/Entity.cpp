#include "pch.h"
#include "Entity.h"

namespace Element {

	Entity::Entity(entt::entity entityID, Scene* scene)
		: entityID(entityID), scene(scene) {}
}
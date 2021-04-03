#include "pch.h"
#include "Entity.h"

namespace Element {

	Entity::Entity(entt::entity entityId, Scene* scene)
		: entityId(entityId), scene(scene) {}
}
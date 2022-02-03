#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include "ScriptableEntity.h"
#include "Components.h"
#include "Entity.h"
#include "Element/Renderer/Renderer2D.h"
#include "Element/Core/GUID.h"

namespace Element {

	static b2BodyType RigidbodyTypeConvertion(Rigidbody2DComponent::BodyType bodyType) {
		switch(bodyType) {
			case Rigidbody2DComponent::BodyType::Static:
				return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:
				return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic:
				return b2_kinematicBody;
			default:
				EL_CORE_FAIL("Unknown type: {0}", bodyType);
		}
	}

	static Rigidbody2DComponent::BodyType RigidbodyTypeConvertion(b2BodyType bodyType) {
		switch(bodyType) {
			case b2_staticBody:
				return Rigidbody2DComponent::BodyType::Static;
			case b2_dynamicBody:
				return Rigidbody2DComponent::BodyType::Dynamic;
			case b2_kinematicBody:
				return Rigidbody2DComponent::BodyType::Kinematic;
			default:
				EL_CORE_FAIL("Unknown type: {0}", bodyType);
		}
	}

	template<typename T>
	static void CopyComponent(entt::registry& dst, entt::registry& src, std::unordered_map<GUID, entt::entity>& enttMap) {
		auto view = src.view<T>();
		for(auto entity : view) {
			//The GUID of the entity to copy
			GUID guid = src.get<IDComponent>(entity).guid;
			//The component to copy
			auto& component = src.get<T>(entity);
			//Finds the destination entity from the guid in the map
			EL_CORE_ASSERT(enttMap.find(guid) != enttMap.end(), "Entity with GUID {0} could not be found in the map! Make sure that all coppied entities were added in Scene::Copy().", guid);
			entt::entity dstEnttID = enttMap.at(guid);
			
			//Copy the componet into the destination
			dst.emplace_or_replace<T>(dstEnttID, component);
		}
	}

	template<typename T>
	static void CopyComponent(Entity dst, Entity src) {
		if(!src.HasComponent<T>()) return;

		dst.AddOrReplaceComponent<T>(src.GetComponent<T>());
	}

	Scene::Scene() {}
	
	Scene::~Scene() {}

	Ref<Scene> Scene::Copy(Ref<Scene> other) {
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->viewportWidth = other->viewportWidth;
		newScene->viewportHeight = other->viewportHeight;

		std::unordered_map<GUID, entt::entity> enttMap;

		auto& srcSceneRegistry = other->registry;
		auto& dstSceneRegistry = newScene->registry;
		auto idView = srcSceneRegistry.view<IDComponent>();

		//Create the entities in the new scene
		for(auto item : idView)
		{
			GUID guid = srcSceneRegistry.get<IDComponent>(item).guid;
			const std::string& name = srcSceneRegistry.get<TagComponent>(item).tag;
			Entity newEntity = newScene->CreateEntity(guid, name);

			//Add the entity to the map
			enttMap[guid] = static_cast<entt::entity>(newEntity);
		}

		//Todo: Replace with a component type look up
		//Copy all components except IDComponent and TagComponent
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CircleRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	void Scene::OnUpdateRuntime(Timestep ts) {
		EL_PROFILE_FUNCTION();

		//Update script functions
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if(!nsc.instance) {
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->entity = Entity{ entity, this };
					nsc.instance->OnCreate();
				}

				nsc.instance->OnUpdate(ts);
			});
		}

		//Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			physicsWorld->Step(ts, velocityIterations, positionIterations);

			//Get transform from box2d
			auto view = registry.view<Rigidbody2DComponent>();
			for(auto item : view) {
				Entity entity = { item, this };
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				Rigidbody2DComponent& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = reinterpret_cast<b2Body*>(rigidbody.runtimeBody);
				const b2Vec2& position = body->GetPosition();
				//Apply the new position (Physics)
				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation.z = body->GetAngle();
			}
		}

		//Setup camera
		Camera* mainCamera = nullptr;
		glm::mat4 mainCameraTransform;
		{
			auto view = registry.view<TransformComponent, CameraComponent>();
			for(auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if(camera.primary) {
					mainCamera = &camera.camera;
					mainCameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		//Begin the scene and draw all objects
		if(mainCamera) {
			Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

			//Render all sprite renderer components
			{
				auto view = registry.view<TransformComponent, SpriteRendererComponent>();
				for(auto entity : view) {
					auto [transform, renderer] = view.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), renderer, static_cast<int>(entity));
				}
			}

			//Render all circle renderer components
			{
				auto view = registry.view<TransformComponent, CircleRendererComponent>();
				for(auto entity : view) {
					auto [transform, renderer] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::DrawCircle(transform.GetTransform(), renderer, static_cast<int>(entity));
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		EL_PROFILE_FUNCTION();

		Renderer2D::BeginScene(camera);

		//Render all sprite renderer components
		{
			auto view = registry.view<TransformComponent, SpriteRendererComponent>();
			for(auto entity : view) {
				auto [transform, renderer] = view.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), renderer, static_cast<int>(entity));
			}
		}

		//Render all circle renderer components
		{
			auto view = registry.view<TransformComponent, CircleRendererComponent>();
			for(auto entity : view) {
				auto [transform, renderer] = view.get<TransformComponent, CircleRendererComponent>(entity);

				Renderer2D::DrawCircle(transform.GetTransform(), renderer, static_cast<int>(entity));
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		viewportWidth = width;
		viewportHeight = height;

		auto view = registry.view<CameraComponent>();
		for(auto entity : view) {
			auto& camera = view.get<CameraComponent>(entity);

			if(!camera.fixedAspectRatio)
				camera.camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnRuntimeStart() {
		physicsWorld = new b2World({ 0.0f, -9.8 });

		auto view = registry.view<Rigidbody2DComponent>();
		for(auto item : view) {
			Entity entity = { item, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = RigidbodyTypeConvertion(rigidbody.bodyType);
			bodyDef.position.Set(transform.translation.x, transform.translation.y);
			bodyDef.angle = transform.rotation.z;

			b2Body* body = physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rigidbody.fixedRotation);
			rigidbody.runtimeBody = body;

			//Todo: Try get
			if(entity.HasComponent<BoxCollider2DComponent>()) {
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				//Todo: Add option to disable scale
				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.density;
				fixtureDef.friction = bc2d.friction;
				fixtureDef.restitution = bc2d.restitution;
				fixtureDef.restitutionThreshold = bc2d.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if(entity.HasComponent<CircleCollider2DComponent>()) {
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
				circleShape.m_radius = cc2d.radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.density;
				fixtureDef.friction = cc2d.friction;
				fixtureDef.restitution = cc2d.restitution;
				fixtureDef.restitutionThreshold = cc2d.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop() {
		delete physicsWorld;
		physicsWorld = nullptr;
	}

	void Scene::DuplicateEntity(Entity entity) {
		std::string name = entity.GetName() + " (Duplicate)";
		Entity newEntity = CreateEntity(name);

		//Todo: Replace with a component type look up
		CopyComponent<TransformComponent>(newEntity, entity);
		CopyComponent<SpriteRendererComponent>(newEntity, entity);
		CopyComponent<CircleRendererComponent>(newEntity, entity);
		CopyComponent<CameraComponent>(newEntity, entity);
		CopyComponent<NativeScriptComponent>(newEntity, entity);
		CopyComponent<Rigidbody2DComponent>(newEntity, entity);
		CopyComponent<BoxCollider2DComponent>(newEntity, entity);
		CopyComponent<CircleCollider2DComponent>(newEntity, entity);
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = registry.view<CameraComponent>();
		for(auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if(camera.primary)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::CreateEntity(const std::string& name) {
		return CreateEntity(GUID(), name);
	}

	Entity Scene::CreateEntity(GUID guid, const std::string& name) {
		EL_PROFILE_FUNCTION();

		Entity entity = { registry.create(), this };

		entity.AddComponent<IDComponent>(guid);

		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag = !name.empty() ? name : std::string("Entity");

		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		EL_PROFILE_FUNCTION();

		registry.destroy(entity);
	}

	#pragma region OnComponentAdded<>() Definitions

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false, "Static assert failed! Scene::OnComponentAdded() does not currently support generic types. If you added a new component you probably just forgot to create a manual type below.");
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if(viewportWidth > 0 && viewportHeight > 0)
			component.camera.SetViewportSize(viewportWidth, viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {}

	#pragma endregion
}

#include "pch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include "Components.h"
#include "Entity.h"
#include "Element/Renderer/Renderer2D.h"

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

	Scene::Scene() {}
	
	Scene::~Scene() {}

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

			//Render all sprite components
			auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		EL_PROFILE_FUNCTION();

		Renderer2D::BeginScene(camera);

		//Render all sprite components
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for(auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
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
		}
	}

	void Scene::OnRuntimeStop() {
		delete physicsWorld;
		physicsWorld = nullptr;
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
		EL_PROFILE_FUNCTION();

		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag = !name.empty() ? name : std::string("Entity");
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		EL_PROFILE_FUNCTION();

		registry.destroy(entity);
	}

	#pragma region OnComponentAdded<>() Definitions

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		//We dont support generic types. Instead define below.
		static_assert(false);
	}

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
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}

	#pragma endregion
}

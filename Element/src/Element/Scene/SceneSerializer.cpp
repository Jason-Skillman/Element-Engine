#include "pch.h"
#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"
#include "Element/Utils/YAMLExtentions.h"

namespace Element {

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: scene(scene) {}
	
	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		EL_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Every entity must have an IDComponent!");

		out << YAML::BeginMap;
		{
			out << YAML::Key << "Entity" << YAML::Value << entity.GetGUID();

			//Tag component
			if(entity.HasComponent<TagComponent>()) {
				out << YAML::Key << "TagComponent";

				out << YAML::BeginMap;
				{
					TagComponent& component = entity.GetComponent<TagComponent>();

					out << YAML::Key << "Tag" << YAML::Value << component.tag;
				}
				out << YAML::EndMap;
			}

			//Transform component
			if(entity.HasComponent<TransformComponent>()) {
				out << YAML::Key << "TransformComponent";

				out << YAML::BeginMap;
				{
					TransformComponent& component = entity.GetComponent<TransformComponent>();

					out << YAML::Key << "Translation" << YAML::Value << component.translation;
					out << YAML::Key << "Rotation" << YAML::Value << component.rotation;
					out << YAML::Key << "Scale" << YAML::Value << component.scale;
				}
				out << YAML::EndMap;
			}

			//Camera component
			if(entity.HasComponent<CameraComponent>()) {
				out << YAML::Key << "CameraComponent";

				out << YAML::BeginMap;
				{
					CameraComponent& component = entity.GetComponent<CameraComponent>();
					SceneCamera& camera = component.camera;

					out << YAML::Key << "Camera" << YAML::Value;
					out << YAML::BeginMap;
					{
						out << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(camera.GetProjectionType());
						out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
						out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
						out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
						out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
						out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
						out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
					}
					out << YAML::EndMap;

					out << YAML::Key << "Primary" << YAML::Value << component.primary;
					out << YAML::Key << "FixedAspectRation" << YAML::Value << component.fixedAspectRatio;
				}
				out << YAML::EndMap;
			}

			//Sprite renderer component
			if(entity.HasComponent<SpriteRendererComponent>()) {
				out << YAML::Key << "SpriteRendererComponent";

				out << YAML::BeginMap;
				{
					SpriteRendererComponent& component = entity.GetComponent<SpriteRendererComponent>();

					out << YAML::Key << "Color" << YAML::Value << component.color;
				}
				out << YAML::EndMap;
			}

			//Rigidbody 2D component
			if(entity.HasComponent<Rigidbody2DComponent>()) {
				out << YAML::Key << "Rigidbody2DComponent";

				out << YAML::BeginMap;
				{
					Rigidbody2DComponent& component = entity.GetComponent<Rigidbody2DComponent>();

					out << YAML::Key << "BodyType" << YAML::Value << static_cast<int>(component.bodyType);
					out << YAML::Key << "FixedRotation" << YAML::Value << component.fixedRotation;
				}
				out << YAML::EndMap;
			}

			//Box collider 2D component
			if(entity.HasComponent<BoxCollider2DComponent>()) {
				out << YAML::Key << "BoxCollider2DComponent";

				out << YAML::BeginMap;
				{
					BoxCollider2DComponent& component = entity.GetComponent<BoxCollider2DComponent>();

					out << YAML::Key << "Offset" << YAML::Value << component.offset;
					out << YAML::Key << "Size" << YAML::Value << component.size;
					out << YAML::Key << "Density" << YAML::Value << component.density;
					out << YAML::Key << "Friction" << YAML::Value << component.friction;
					out << YAML::Key << "Restitution" << YAML::Value << component.restitution;
					out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;
				}
				out << YAML::EndMap;
			}
		}
		out << YAML::EndMap;
	}
	
	void SceneSerializer::Serialize(const std::filesystem::path& filepath) {
		YAML::Emitter out;
	
		out << YAML::BeginMap;

		//Get filename without extention
		const std::string sceneNameWithExtention(filepath.filename().string());
		size_t lastindex = sceneNameWithExtention.find_last_of(".");
		std::string sceneName = sceneNameWithExtention.substr(0, lastindex);
	
		out << YAML::Key << "Scene" << YAML::Value << sceneName;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	
		scene->registry.each([&](auto entityID) {
			Entity entity = { entityID, scene.get() };
			if(!entity)
				return;
		
			SerializeEntity(out, entity);
		});
	
		out << YAML::EndSeq;
		out << YAML::EndMap;
	
		std::ofstream fout(filepath.string());
		fout << out.c_str();

		//EL_LOG_CORE_TRACE("Serialized scene {0}", sceneName);
	}
	
	void SceneSerializer::SerializBinary(const std::filesystem::path& filepath) {
		throw "NotImplementedException";
	}
	
	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if(!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		scene->SetName(sceneName);

		//EL_LOG_CORE_TRACE("Deserialized scene {0}", sceneName);

		YAML::Node entities = data["Entities"];
		if(entities) {

			//Loop through all entities in the scene
			for(auto entity : entities) {

				uint64_t guid = entity["Entity"].as<uint64_t>();

				std::string name;
				YAML::Node tagComponent = entity["TagComponent"];
				if(tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				//Create the entity
				Entity newEntity = scene->CreateEntity(guid, name);

				//Transform component
				{
					YAML::Node componentValue = entity["TransformComponent"];
					if(componentValue) {
						TransformComponent& component = newEntity.GetComponent<TransformComponent>();

						component.translation = componentValue["Translation"].as<glm::vec3>();
						component.rotation = componentValue["Rotation"].as<glm::vec3>();
						component.scale = componentValue["Scale"].as<glm::vec3>();
					}
				}

				//Camera component
				{
					YAML::Node componentValue = entity["CameraComponent"];
					if(componentValue) {
						CameraComponent& component = newEntity.AddComponent<CameraComponent>();
						YAML::Node& cameraProps = componentValue["Camera"];

						component.camera.SetProjectionType((ProjectionType)cameraProps["ProjectionType"].as<int>());

						component.camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
						component.camera.SetPerspectiveNearClip(cameraProps["PerspectiveNearClip"].as<float>());
						component.camera.SetPerspectiveFarClip(cameraProps["PerspectiveFarClip"].as<float>());

						component.camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
						component.camera.SetOrthographicNearClip(cameraProps["OrthographicNearClip"].as<float>());
						component.camera.SetOrthographicFarClip(cameraProps["OrthographicFarClip"].as<float>());

						component.primary = componentValue["Primary"].as<bool>();
						component.fixedAspectRatio = componentValue["FixedAspectRation"].as<bool>();
					}
				}

				//Sprite renderer component
				{
					YAML::Node componentValue = entity["SpriteRendererComponent"];
					if(componentValue) {
						SpriteRendererComponent& component = newEntity.AddComponent<SpriteRendererComponent>();

						component.color = componentValue["Color"].as<glm::vec4>();
					}
				}

				//Rigidbody 2D component
				{
					YAML::Node componentValue = entity["Rigidbody2DComponent"];
					if(componentValue) {
						Rigidbody2DComponent& component = newEntity.AddComponent<Rigidbody2DComponent>();

						component.bodyType = static_cast<Rigidbody2DComponent::BodyType>(componentValue["BodyType"].as<int>());
						component.fixedRotation = componentValue["FixedRotation"].as<bool>();
					}
				}

				//Box collider 2D component
				{
					YAML::Node componentValue = entity["BoxCollider2DComponent"];
					if(componentValue) {
						BoxCollider2DComponent& component = newEntity.AddComponent<BoxCollider2DComponent>();

						component.offset = componentValue["Offset"].as<glm::vec2>();
						component.size = componentValue["Size"].as<glm::vec2>();
						component.density = componentValue["Density"].as<float>();
						component.friction = componentValue["Friction"].as<float>();
						component.restitution = componentValue["Restitution"].as<float>();
						component.restitutionThreshold = componentValue["RestitutionThreshold"].as<float>();
					}
				}
			}
		}
		
		return true;
	}
	
	bool SceneSerializer::DeserializBinary(const std::filesystem::path& filepath) {
		throw "NotImplementedException";
		return false;
	}
}

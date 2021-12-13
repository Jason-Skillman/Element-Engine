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
		out << YAML::BeginMap;
		{
			out << YAML::Key << "Entity" << YAML::Value << "12345";	//Todo: Random entity ID goes here

			if(entity.HasComponent<TagComponent>()) {
				out << YAML::Key << "TagComponent";

				out << YAML::BeginMap;
				{
					TagComponent& component = entity.GetComponent<TagComponent>();

					out << YAML::Key << "Tag" << YAML::Value << component.tag;
				}
				out << YAML::EndMap;
			}

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

			if(entity.HasComponent<CameraComponent>()) {
				out << YAML::Key << "CameraComponent";

				out << YAML::BeginMap;
				{
					CameraComponent& component = entity.GetComponent<CameraComponent>();
					SceneCamera& camera = component.camera;

					out << YAML::Key << "Camera" << YAML::Value;
					out << YAML::BeginMap;
					{
						out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
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

			if(entity.HasComponent<SpriteRendererComponent>()) {
				out << YAML::Key << "SpriteRendererComponent";

				out << YAML::BeginMap;
				{
					SpriteRendererComponent& component = entity.GetComponent<SpriteRendererComponent>();

					out << YAML::Key << "Color" << YAML::Value << component.color;
				}
				out << YAML::EndMap;
			}
		}
		out << YAML::EndMap;
	}
	
	void SceneSerializer::Serialize(const std::string& filepath) {
		YAML::Emitter out;
	
		out << YAML::BeginMap;

		const std::string sceneName("Untitled");
	
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
	
		std::ofstream fout(filepath);
		fout << out.c_str();

		EL_LOG_CORE_TRACE("Serialized scene {0}", sceneName);
	}
	
	void SceneSerializer::SerializBinary(const std::string& filepath) {}
	
	bool SceneSerializer::Deserialize(const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if(!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		EL_LOG_CORE_TRACE("Deserialized scene {0}", sceneName);

		YAML::Node entities = data["Entities"];
		if(entities) {

			for(auto entity : entities) {

				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				YAML::Node tagComponent = entity["TagComponent"];
				if(tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				EL_LOG_CORE_TRACE("Deserialized entity ID = {0}, Name = {1}", uuid, name);

				//Create the entity
				Entity newEntity = scene->CreateEntity(name);

				//Setup transform component. All entities have a transform component.
				{
					YAML::Node componentValue = entity["TransformComponent"];
					if(componentValue) {
						TransformComponent& component = newEntity.GetComponent<TransformComponent>();

						component.translation = componentValue["Translation"].as<glm::vec3>();
						component.rotation = componentValue["Rotation"].as<glm::vec3>();
						component.scale = componentValue["Scale"].as<glm::vec3>();
					}
				}

				//Setup camera component
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

				//Setup sprite renderer component
				{
					YAML::Node componentValue = entity["SpriteRendererComponent"];
					if(componentValue) {
						SpriteRendererComponent& component = newEntity.AddComponent<SpriteRendererComponent>();

						component.color = componentValue["Color"].as<glm::vec4>();
					}
				}

			}
		}
		
		return true;
	}
	
	bool SceneSerializer::DeserializBinary(const std::string& filepath) {
		throw "NotImplementedException";
		return false;
	}
}

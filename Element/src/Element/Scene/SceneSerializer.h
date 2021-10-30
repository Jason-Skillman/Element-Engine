#pragma once

#include "Scene.h"
#include "Element/Core/Core.h"

namespace Element {

	class SceneSerializer {
	private:
		Ref<Scene> scene;

	public:
		SceneSerializer(const Ref<Scene>& scene);

	public:
		void Serialize(const std::string& filepath);
		void SerializBinary(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializBinary(const std::string& filepath);
	};
}

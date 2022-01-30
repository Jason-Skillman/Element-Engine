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
		void Serialize(const std::filesystem::path& filepath);
		void SerializBinary(const std::filesystem::path& filepath);

		bool Deserialize(const std::filesystem::path& filepath);
		bool DeserializBinary(const std::filesystem::path& filepath);
	};
}

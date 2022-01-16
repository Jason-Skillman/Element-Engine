#pragma once

#include <filesystem>

#include "Element/Renderer/Texture.h"

namespace Element {

	class ContentBrowserPanel {
	private:
		std::filesystem::path currentDirectory;

		Ref<Texture2D> directoryIcon, fileIcon;

	public:
		ContentBrowserPanel();

	public:
		void OnImGuiRender();
	};
}

#pragma once

#include <filesystem>

namespace Element {

	class ContentBrowserPanel {
	private:
		std::filesystem::path currentDirectory;

	public:
		ContentBrowserPanel();

	public:
		void OnImGuiRender();
	};
}

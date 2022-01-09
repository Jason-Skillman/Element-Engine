#include "ContentBrowserPanel.h"

#include <filesystem>

#include <Element.h>
#include <imgui.h>

namespace Element {

	//Todo: Change directory when project locations are added
	static const std::filesystem::path assetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: currentDirectory(assetsPath) {}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		//Display current file path
		//ImGui::Text("Path: %s", currentDirectory.string());

		//Only display back button if you are navigating through the assets directory. Prevent user from navigating outside of the project.
		if(currentDirectory != std::filesystem::path(assetsPath)) {
			//Create a directory back button
			if(ImGui::Button("<-")) {
				currentDirectory = currentDirectory.parent_path();
			}
		}

		//Iterate through all files and display them
		for(auto& dirEntry : std::filesystem::directory_iterator(currentDirectory)) {
			//Path of current element
			const auto& path = dirEntry.path();
			std::string pathStr = path.string();

			auto filename = path.filename();
			auto filenameStr = filename.string();

			//Alternateive to above filename. Displays parent folder relative to current path.
			//auto relativePath = std::filesystem::relative(path, assetsPath);
			//std::string relativePathStr = relativePath.string();
			
			//Check if path is a directory
			if(dirEntry.is_directory()) {
				//Display folder
				if(ImGui::Button(filenameStr.c_str())) {
					//Navigate inside the folder
					
					currentDirectory /= filename;
				}
			} else {
				//Display file
				if(ImGui::Button(filenameStr.c_str())) {
					
				}
			}
		}

		ImGui::End();
	}
}

#include "ContentBrowserPanel.h"

#include <filesystem>

#include <Element.h>
#include <imgui.h>

namespace Element {

	//Todo: Change directory when project locations are added
	static const std::filesystem::path assetsPath = "Assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: currentDirectory(assetsPath) {
	
		directoryIcon = Texture2D::Create("Resources/Icons/directory_icon.png");
		fileIcon = Texture2D::Create("Resources/Icons/file_icon.png");
	}

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

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if(columnCount < 1)
			columnCount = 1;

		{
			ImGui::Columns(columnCount, 0, false);

			//Iterate through all files and display them
			for(auto& dirEntry : std::filesystem::directory_iterator(currentDirectory)) {
				//Path of current element
				const auto& path = dirEntry.path();
				std::string pathStr = path.string();

				auto filename = path.filename();
				std::string filenameStr = filename.string();

				//Alternateive to above filename. Displays parent folder relative to current path.
				//auto relativePath = std::filesystem::relative(path, assetsPath);
				//std::string relativePathStr = relativePath.string();

				//Switch image depending on directory or file
				Ref<Texture2D> icon = dirEntry.is_directory() ? directoryIcon : fileIcon;
				ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				//Button click
				if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					//Check if path is a directory
					if(dirEntry.is_directory()) {
						//Navigate inside the folder
						currentDirectory /= filename;
					}
				}

				//Display the filename
				ImGui::TextWrapped(filenameStr.c_str());

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
			//ImGui::SliderFloat("Padding", &padding, 0, 32);
		}

		ImGui::End();
	}
}

#include "SpriteSheetLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Element/Debug/Instrumentor.h"

static const char* tiles = 
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwddwddwwwwwwwwwwww"
"wwwwdddddddddddwwwwwwwww"
"wwwddddddddddddddwwwwwww"
"wwddwwwdddddddddddddwwww"
"wdddwwwddddddddddddddwww"
"wwdddddddddddddddddddwww"
"wwddddddddddddddddddwwww"
"wwwdddddddddddddddwwwwww"
"wwwwwddddddddddddwwwwwww"
"wwwwwwdddddddddwwwwwwwww"
"wwwwwwwdddddddwwwwwwwwww"
"wwwwwwwwdddddwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww";

SpriteSheetLayer::SpriteSheetLayer()
	: Layer("Sprite Sheet"), cameraController(16.0f / 9.0f) {}

void SpriteSheetLayer::OnAttach() {
	textureRPGSpriteSheet = Element::Texture2D::Create("assets/textures/rpg_pack_sprite_sheet.png");

	texStairsLeft = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 7, 6 }, { 128, 128 });
	texBarrel = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 8, 2 }, { 128, 128 });
	texTreeOrange = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	textureMap['w'] = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 11, 11 }, { 128, 128 });
	textureMap['d'] = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 6, 11 }, { 128, 128 });

	cameraController.SetZoomLevel(5.0f);
}

void SpriteSheetLayer::OnDetach() {}

void SpriteSheetLayer::OnUpdate(Element::Timestep ts) {
	EL_PROFILE_FUNCTION();

	//Update
	{
		EL_PROFILE_SCOPE("Update");

		cameraController.OnUpdate(ts);
	}

	//Pre-render
	{
		EL_PROFILE_SCOPE("Pre-render");

		Element::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Element::RenderCommand::Clear();
	}

	//Rendering
	{
		EL_PROFILE_SCOPE("Rendering");

		Element::Renderer2D::ResetStats();
		Element::Renderer2D::BeginScene(cameraController.GetCamera());

		{
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.0f };
			Element::Renderer2D::DrawQuad(drawProps, texStairsLeft);
		}

		{
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { 1.0f, 0.0f, 0.0f };
			Element::Renderer2D::DrawQuad(drawProps, texBarrel);
		}

		{
			Element::Renderer2D::DrawProperties drawProps;
			drawProps.position = { -1.0f, 0.5f, 0.0f };
			drawProps.scale = { 1.0f, 1.0f * 2 };
			Element::Renderer2D::DrawQuad(drawProps, texTreeOrange);
		}

		//Draw the tile map
		{
			for(uint32_t y = 0; y < mapHeight; y++) {
				for(uint32_t x = 0; x < mapWidth; x++) {
					Element::Renderer2D::DrawProperties drawProps;
					drawProps.position = { x, mapHeight - y, 0.0f };

					char tileType = tiles[x + y * mapWidth];
					Element::Ref<Element::SubTexture2D> texture;

					//Check if the tile type was found
					if(textureMap.find(tileType) != textureMap.end()) {
						texture = textureMap[tileType];
					} else {
						EL_LOG_ERROR("Tile type could not be found.");
						return;
					}

					Element::Renderer2D::DrawQuad(drawProps, texture);
				}
			}
		}

		Element::Renderer2D::EndScene();
	}
}

void SpriteSheetLayer::OnImGuiRender() {
	ImGui::Begin("Renderer 2D Stats");

	auto stats = Element::Renderer2D::GetStats();
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Textures loaded: %d", stats.texturesLoaded);

	ImGui::End();
}

void SpriteSheetLayer::OnEvent(Element::Event& event) {
	cameraController.OnEvent(event);
}

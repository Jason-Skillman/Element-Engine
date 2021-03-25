#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Element/Debug/Instrumentor.h"

static const char* tiles = "wwwwwwwwwwwwwwwwwwwwwwww"
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

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(16.0f / 9.0f) {}

void SandboxLayer::OnAttach() {
	textureCheckerboard = Element::Texture2D::Create("assets/textures/checkerboard.png");
	textureArrow = Element::Texture2D::Create("assets/textures/arrow_head.png");
	textureRPGSpriteSheet = Element::Texture2D::Create("assets/textures/rpg_pack_sprite_sheet.png");

	texStairsLeft = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 7, 6 }, { 128, 128 });
	texBarrel = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 8, 2 }, { 128, 128 });
	texTreeOrange = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	textureMap['w'] = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 11, 11 }, { 128, 128 });
	textureMap['d'] = Element::SubTexture2D::CreateFromCoords(textureRPGSpriteSheet, { 6, 11 }, { 128, 128 });

	particleProps.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	particleProps.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	particleProps.sizeBegin = 0.5f, particleProps.sizeVariation = 0.3f, particleProps.sizeEnd = 0.0f;
	particleProps.lifeTime = 1.0f;
	particleProps.velocity = { 0.0f, 0.0f };
	particleProps.velocityVariation = { 3.0f, 1.0f };
	particleProps.position = { 0.0f, 0.0f };

	cameraController.SetZoomLevel(5.0f);
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Element::Timestep ts) {
	PROFILE_FUNCTION();
	
	//Update
	{
		PROFILE_SCOPE("Update");
		
		cameraController.OnUpdate(ts);
	}

	//Pre-render
	{
		PROFILE_SCOPE("Pre-render");
		
		Element::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Element::RenderCommand::Clear();
	}

	//Rendering
	{
		PROFILE_SCOPE("Rendering");

		Element::Renderer2D::ResetStats();
		Element::Renderer2D::BeginScene(cameraController.GetCamera());

		//Draw the tile map
		{
			for(uint32_t y = 0; y < mapHeight; y++) {
				for(uint32_t x = 0; x < mapWidth; x++) {
					Element::Renderer2D::DrawProporties drawProps;
					drawProps.position = { x, mapHeight - y, 0.0f };

					char tileType = tiles[x + y * mapWidth];
					Element::Ref<Element::SubTexture2D> texture;

					//Check if the tile type was found
					if(textureMap.find(tileType) != textureMap.end()) {
						texture = textureMap[tileType];
					} else {
						LOG_ERROR("Tile type could not be found.");
						return;
					}
					
					Element::Renderer2D::DrawQuad(drawProps, texture);
				}
			}
		}

		
		/*{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.0f };
			Element::Renderer2D::DrawQuad(drawProps, texStairsLeft);
		}

		{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 1.0f, 0.0f, 0.0f };
			Element::Renderer2D::DrawQuad(drawProps, texBarrel);
		}

		{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { -1.0f, 0.5f, 0.0f };
			drawProps.scale = { 1.0f, 1.0f * 2 };
			Element::Renderer2D::DrawQuad(drawProps, texTreeOrange);
		}*/

		//Red square
		/*{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.1f };
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.8f, 0.2f, 0.3f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}*/

		//Green square
		/*{
			static float rotation = 0.0f;
			rotation += ts * 20.0f;
			
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.6f, 0.0f, 0.2f };
			drawProps.rotation = glm::radians(rotation);
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.3f, 0.8f, 0.2f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}*/

		//Blue square
		/*{
			static float rotation = 0.0f;
			rotation += ts * 45.0f;
			
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 1.2f, 0.0f, 0.3f };
			drawProps.rotation = glm::radians(rotation);
			drawProps.scale = { 0.4f, 0.4f };
			drawProps.color = { 0.2f, 0.3f, 0.8f, 1.0f };
			Element::Renderer2D::DrawQuad(drawProps);
		}*/

		//Texture
		/*{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, 0.4f };
			drawProps.scale = { 0.3f, 0.3f };
			Element::Renderer2D::DrawQuad(drawProps, textureArrow);
		}*/

		//Background
		/*{
			Element::Renderer2D::DrawProporties drawProps;
			drawProps.position = { 0.0f, 0.0f, -0.1f };
			drawProps.scale = { 10.0f, 10.0f };
			drawProps.texture = textureCheckerboard;
			drawProps.tiling = 20.0f;
			Element::Renderer2D::DrawQuad(drawProps);
		}*/
		
		Element::Renderer2D::EndScene();

		//Particles
		/*if(Element::Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			auto [x, y] = Element::Input::GetMousePosition();
			auto width = Element::Application::GetInstance().GetWindow().GetWidth();
			auto height = Element::Application::GetInstance().GetWindow().GetHeight();

			auto bounds = cameraController.GetBounds();
			auto pos = cameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			particleProps.position = { x + pos.x, y + pos.y };
			particleProps.sizeBegin = 0.1f;
			for(int i = 0; i < 5; i++)
				particleSystem.Emit(particleProps);
		}

		particleSystem.OnUpdate(ts);
		particleSystem.OnRender(cameraController.GetCamera());*/
	}
}

void SandboxLayer::OnImGuiRender() {
	/*ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();*/

	/*ImGui::Begin("Profiler");

	for(auto& result : profileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	profileResults.clear();

	ImGui::End();*/

	ImGui::Begin("Renderer 2D Stats");

	auto stats = Element::Renderer2D::GetStats();
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Textures loaded: %d", stats.texturesLoaded);

	ImGui::End();
}

void SandboxLayer::OnEvent(Element::Event& event) {
	cameraController.OnEvent(event);
}

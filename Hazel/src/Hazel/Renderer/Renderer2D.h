#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {
	/// <summary>
	/// Static class
	/// </summary>
	class Renderer2D {
	public:
		struct DrawProporties {
			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			/// <summary>
			/// The amount to rotate in degrees.
			/// </summary>
			float rotation = 0.0f;
			glm::vec2 scale = { 1.0f, 1.0f };

			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			
			Ref<Texture> texture;
			float tilingFactor = 1.0f;
		};
		
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		
		static void DrawQuad(const DrawProporties& properties);
		
	};
}

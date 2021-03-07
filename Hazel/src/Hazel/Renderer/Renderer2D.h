#pragma once

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

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

	private:
		struct QuadVertex {
			glm::vec3 position;
			glm::vec2 texCoord;
			glm::vec4 color;
		};
		
		struct RendererData {
			const uint32_t maxQuads = 10000;
			const uint32_t maxVertices = maxQuads * 4;
			const uint32_t maxIndices = maxQuads * 6;
			static const uint32_t maxTextureSlots = 32; //Todo: Render caps
			
			Ref<VertexArray> quadVertexArray;
			Ref<VertexBuffer> quadVertexBuffer;
			Ref<Shader> textureShader;
			Ref<Texture2D> whiteTexture;

			uint32_t quadIndexCount = 0;
			QuadVertex* quadVertexBufferBase = nullptr;
			QuadVertex* quadVertexBufferPtr = nullptr;

			std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
			uint32_t textureSlotIndex = 1;
		};

	private:
		static RendererData data;
		
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		
		static void DrawQuad(const DrawProporties& properties);
	};
}

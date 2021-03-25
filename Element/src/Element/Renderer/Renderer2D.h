#pragma once

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "SubTexture2D.h"

namespace Element {
	/// <summary>
	/// Static class
	/// </summary>
	class Renderer2D {
	public:
		struct DrawProporties {
			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			/// <summary>
			/// The amount to rotate in radians.
			/// </summary>
			float rotation = 0.0f;
			glm::vec2 scale = { 1.0f, 1.0f };

			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			
			
			float tiling = 1.0f;
		};

	private:
		struct Statistics {
			uint32_t drawCalls;
			uint32_t quadCount;
			uint32_t texturesLoaded;

			uint32_t GetTotalVertexCount() {
				return quadCount * 4;
			}

			uint32_t GetTotalIndexCount() {
				return quadCount * 6;
			}
		};
		
		struct QuadVertex {
			glm::vec3 position;
			glm::vec2 texCoord;
			glm::vec4 color;
			float textureIndex;
			float tiling;
		};
		
		struct RendererData {
			const uint32_t maxQuads = 1000;
			const uint32_t maxVertices = maxQuads * 4;
			const uint32_t maxIndices = maxQuads * 6;
			static const uint32_t maxTextureSlots = 32; //Todo: Render caps
			
			Ref<VertexArray> quadVertexArray;
			Ref<VertexBuffer> quadVertexBuffer;
			Ref<Shader> standardShader;
			Ref<Texture2D> whiteTexture;

			uint32_t quadIndexCount = 0;
			QuadVertex* quadVertexBufferBase = nullptr;
			QuadVertex* quadVertexBufferPtr = nullptr;

			std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
			uint32_t textureSlotIndex = 1;

			glm::vec4 quadVertexPositions[4];

			Statistics stats;
		};

	private:
		static RendererData data;
		
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		
		static void DrawQuad(const DrawProporties& properties, const Ref<Texture2D>& texture = nullptr, const glm::vec2* texCoords = nullptr);
		static void DrawQuad(const DrawProporties& properties, const Ref<SubTexture2D>& subTexture);

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
	};
}

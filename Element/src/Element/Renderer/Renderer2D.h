#pragma once

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "SubTexture2D.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "Element/Scene/Components.h"

namespace Element {

	class Renderer2D {
	public:
		struct DrawProperties {
			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			/// <summary>
			/// The amount to rotate in radians.
			/// </summary>
			float rotation = 0.0f;
			glm::vec2 scale = { 1.0f, 1.0f };
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			float tiling = 1.0f;
			int entityID = -1;
		};

		struct DrawPropertiesMat4 {
			glm::mat4 transform = glm::mat4(1.0f);
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			float tiling = 1.0f;
			int entityID = -1;
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

			//Editor only
			int entityID;
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

		static void ResetStats();
		static Statistics GetStats();
		
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		[[deprecated("Use BeginScene(camera, mat4)")]]
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		
		static void DrawQuad(const DrawProperties& properties, const Ref<Texture2D>& texture = nullptr, const glm::vec2* texCoords = nullptr);
		static void DrawQuad(const DrawProperties& properties, const Ref<SubTexture2D>& subTexture);

		static void DrawQuad(const DrawPropertiesMat4& properties, const Ref<Texture2D>& texture = nullptr, const glm::vec2* texCoords = nullptr);
		static void DrawQuad(const DrawPropertiesMat4& properties, const Ref<SubTexture2D>& subTexture);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& spriteComponent, int entityID = -1);

	private:
		static void StartBatch();
		static void NextBatch();
		static void Flush();
	};
}

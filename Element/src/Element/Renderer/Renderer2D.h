#pragma once

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Element {
	/// <summary>
	/// Static class
	/// </summary>
	class Renderer2D {
	public:
		struct DrawProporties {
			/*struct TexCoord {
				uint32_t x, y, spriteWidth, spriteHeight, maxWidth, maxHeight;

				TexCoord(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t maxWidth, uint32_t maxHeight)
					: x(x), y(y), spriteWidth(width), spriteHeight(height), maxWidth(maxWidth), maxHeight(maxHeight) {}

				glm::vec2* GetTexCoords() {
					glm::vec2 newVec[] = {
						{ (x * spriteWidth) / maxWidth, (y * spriteHeight) / maxHeight },
						{ ((x + 1) * spriteWidth) / maxWidth, (y * spriteHeight) / maxHeight },
						{ ((x + 1) * spriteWidth) / maxWidth, ((y + 1) * spriteHeight) / maxHeight },
						{ (x * spriteWidth) / maxWidth, ((y + 1) * spriteHeight) / maxHeight }
					};
					return newVec;
				}

				glm::vec2 texCoords[4] = {
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.0f, 1.0f }
				};
			};*/
			
			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			/// <summary>
			/// The amount to rotate in radians.
			/// </summary>
			float rotation = 0.0f;
			glm::vec2 scale = { 1.0f, 1.0f };

			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			
			
			float tiling = 1.0f;
			//TexCoord texCoords;

			Ref<Texture2D> texture;

			/*void SetTexture(Ref<Texture2D>& texture) {
				this->texture = texture;
				//texCoords = TexCoord(0, 0, texture->GetWidth(), texture->GetHeight(), texture->GetWidth(), texture->GetHeight());
			}*/
		};

	private:
		struct Statistics {
			uint32_t drawCalls;
			uint32_t quadCount;

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
		
		static void DrawQuad(const DrawProporties& properties);
		static void DrawQuadColor(const DrawProporties& properties);

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
	};
}

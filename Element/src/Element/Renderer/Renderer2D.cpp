#include "pch.h"
#include "Renderer2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Buffer.h"
#include "RenderCommand.h"
#include "Element/Renderer/UniformBuffer.h"

namespace Element {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec4 color;
		float textureIndex;
		float tiling;

		//Editor only
		int entityID;
	};

	struct CircleVertex {
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		//Editor only
		int entityID;
	};

	struct Renderer2DData {
		const uint32_t maxQuads = 1000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; //Todo: Render caps

		//Quad data
		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		//Circle data
		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		uint32_t circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;


		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;

		struct CameraData {
			glm::mat4 viewProjection;
		};
		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
	};

	static Renderer2DData data;

	void Renderer2D::Init() {
		EL_PROFILE_FUNCTION();
		
		//Data
		const BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float, "a_TextureIndex" },
			{ ShaderDataType::Float, "a_Tiling" },

			//Editor only
			{ ShaderDataType::Int, "a_EntityID" },
		};

		//Objects
		data.quadVertexArray = VertexArray::Create();
		data.quadShader = Shader::Create("Assets/Shaders/Renderer2D_Quad.glsl");

		//Setup vertex buffer
		data.quadVertexBuffer = VertexBuffer::Create(data.maxVertices * sizeof(QuadVertex));
		data.quadVertexBuffer->SetLayout(layout);
		data.quadVertexArray->AddVertexBuffer(data.quadVertexBuffer);

		data.quadVertexBufferBase = new QuadVertex[data.maxVertices];

		//Setup index buffer
		uint32_t* quadIndices = new uint32_t[data.maxIndices];

		uint32_t offset = 0;
		for(uint32_t i = 0; i < data.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, data.maxIndices);
		data.quadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] quadIndices;

		

		//Create textures
		data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		
		int32_t samplers[data.maxTextureSlots];
		for(uint32_t i = 0; i < data.maxTextureSlots; i++) {
			samplers[i] = i;
		}


		data.textureSlots[0] = data.whiteTexture;

		data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		data.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);

		//Circle data
		{
			const BufferLayout layout = {
				{ ShaderDataType::Float3, "a_WorldPosition" },
				{ ShaderDataType::Float3, "a_LocalPosition" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float, "a_Thickness" },
				{ ShaderDataType::Float, "a_Fade" },

				//Editor only
				{ ShaderDataType::Int, "a_EntityID" },
			};

			data.circleVertexArray = VertexArray::Create();
			data.circleShader = Shader::Create("Assets/Shaders/Renderer2D_Circle.glsl");

			//Setup vertex buffer
			data.circleVertexBuffer = VertexBuffer::Create(data.maxVertices * sizeof(CircleVertex));
			data.circleVertexBuffer->SetLayout(layout);
			data.circleVertexArray->AddVertexBuffer(data.circleVertexBuffer);

			data.circleVertexBufferBase = new CircleVertex[data.maxVertices];

			//Setup index buffer
			data.circleVertexArray->SetIndexBuffer(indexBuffer);	//Use quad index buffer
		}
	}
	
	void Renderer2D::Shutdown() {
		EL_PROFILE_FUNCTION();

		delete[] data.quadVertexBufferBase;
	}

	void Renderer2D::ResetStats() {
		memset(&data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return data.stats;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		EL_PROFILE_FUNCTION();

		data.cameraBuffer.viewProjection = camera.GetProjection() * glm::inverse(transform);
		data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		EL_PROFILE_FUNCTION();

		data.cameraBuffer.viewProjection = camera.GetViewProjection();
		data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		EL_PROFILE_FUNCTION();
		
		data.quadShader->Bind();
		data.quadShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::StartBatch() {
		EL_PROFILE_FUNCTION();

		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;

		data.circleIndexCount = 0;
		data.circleVertexBufferPtr = data.circleVertexBufferBase;

		data.textureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		EL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush() {
		EL_PROFILE_FUNCTION();

		//Draw quads
		if(data.quadIndexCount) {
			uint32_t dataSize = (uint32_t)(reinterpret_cast<uint8_t*>(data.quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.quadVertexBufferBase));
			data.quadVertexBuffer->SetData(data.quadVertexBufferBase, dataSize);

			//Bind all textures to their texture slot
			for(uint32_t i = 0; i < data.textureSlotIndex; i++) {
				data.textureSlots[i]->Bind(i);
			}

			data.quadShader->Bind();
			RenderCommand::DrawIndexed(data.quadVertexArray, data.quadIndexCount);
			data.stats.drawCalls++;
		}

		//Draw circles
		if(data.circleIndexCount) {
			uint32_t dataSize = (uint32_t)(reinterpret_cast<uint8_t*>(data.circleVertexBufferPtr) - reinterpret_cast<uint8_t*>(data.circleVertexBufferBase));
			data.circleVertexBuffer->SetData(data.circleVertexBufferBase, dataSize);

			data.circleShader->Bind();
			RenderCommand::DrawIndexed(data.circleVertexArray, data.circleIndexCount);
			data.stats.drawCalls++;
		}
	}

	void Renderer2D::NextBatch() {
		EL_PROFILE_FUNCTION();

		Flush();
		StartBatch();
	}

	#pragma region Draw

	void Renderer2D::DrawQuad(const DrawProperties& properties, const Ref<Texture2D>& texture, const glm::vec2* texCoords) {
		EL_PROFILE_FUNCTION();

		glm::mat4 transform;
		if(properties.rotation == 0) {
			transform =
				glm::translate(glm::mat4(1.0f), properties.position) *
				glm::scale(glm::mat4(1.0f), { properties.scale.x, properties.scale.y, 1.0f });
		} else {
			transform =
				glm::translate(glm::mat4(1.0f), properties.position) *
				glm::rotate(glm::mat4(1.0f), properties.rotation, { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), { properties.scale.x, properties.scale.y, 1.0f });
		}

		DrawPropertiesMat4 drawProps;
		drawProps.transform = transform;
		drawProps.color = properties.color;
		drawProps.tiling = properties.tiling;
		drawProps.entityID = properties.entityID;

		DrawQuad(drawProps, texture, texCoords);
	}

	void Renderer2D::DrawQuad(const DrawProperties& properties, const Ref<SubTexture2D>& subTexture) {
		DrawQuad(properties, subTexture->GetTexture(), subTexture->GetTexCoords());
	}

	void Renderer2D::DrawQuad(const DrawPropertiesMat4& properties, const Ref<Texture2D>& texture, const glm::vec2* texCoords) {
		EL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if(!texCoords) {
			constexpr glm::vec2 texCoordsOne[] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f }
			};
			texCoords = texCoordsOne;
		}

		if(data.quadIndexCount >= data.maxIndices)
			NextBatch();	//Start the next batch

		//Texture batching
		float textureIndex = 0.0f;
		if(texture != nullptr) {
			for(uint32_t i = 1; i < data.textureSlotIndex; i++) {
				//If the textures are equal
				if(*data.textureSlots[i].get() == *texture.get()) {
					textureIndex = static_cast<float>(i);
					break;
				}
			}
			if(textureIndex == 0.0f) {
				textureIndex = static_cast<float>(data.textureSlotIndex);
				data.textureSlots[data.textureSlotIndex] = texture;
				data.textureSlotIndex++;

				data.stats.texturesLoaded++;
			}
		}

		//Setup the vertex buffer
		for(uint32_t i = 0; i < quadVertexCount; i++) {
			data.quadVertexBufferPtr->position = properties.transform * data.quadVertexPositions[i];
			data.quadVertexBufferPtr->texCoord = texCoords[i];
			data.quadVertexBufferPtr->color = properties.color;
			data.quadVertexBufferPtr->textureIndex = textureIndex;
			data.quadVertexBufferPtr->tiling = properties.tiling;
			data.quadVertexBufferPtr->entityID = properties.entityID;
			data.quadVertexBufferPtr++;
		}
		data.quadIndexCount += 6;

		//Stats
		data.stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const DrawPropertiesMat4& properties, const Ref<SubTexture2D>& subTexture) {
		DrawQuad(properties, subTexture->GetTexture(), subTexture->GetTexCoords());
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteComponent, int entityID) {
		EL_PROFILE_FUNCTION();

		DrawPropertiesMat4 drawProps;
		drawProps.transform = transform;
		drawProps.color = spriteComponent.color;
		drawProps.tiling = spriteComponent.tiling;
		drawProps.entityID = entityID;

		DrawQuad(drawProps, spriteComponent.texture);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const CircleRendererComponent& circleComponent, int entityID) {
		DrawCircle(transform, circleComponent.color, circleComponent.thickness, circleComponent.fade, entityID);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID) {
		EL_PROFILE_FUNCTION();

		//Todo:
		//if(data.quadIndexCount >= data.maxIndices)
		//	NextBatch();	//Start the next batch

		//Setup the vertex buffer
		const int vertexCount = 4;
		for(uint32_t i = 0; i < vertexCount; i++) {
			data.circleVertexBufferPtr->worldPosition = transform * data.quadVertexPositions[i];
			data.circleVertexBufferPtr->localPosition = data.quadVertexPositions[i] * 2.0f;
			data.circleVertexBufferPtr->color = color;
			data.circleVertexBufferPtr->thickness = thickness;
			data.circleVertexBufferPtr->fade = fade;
			data.circleVertexBufferPtr->entityID = entityID;
			data.circleVertexBufferPtr++;
		}
		data.circleIndexCount += 6;

		//Stats
		data.stats.quadCount++;
	}

	#pragma endregion
}

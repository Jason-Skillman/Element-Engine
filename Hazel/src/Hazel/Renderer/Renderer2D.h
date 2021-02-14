#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {
	/// <summary>
	/// Static class
	/// </summary>
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		
		//Primitives
		static void DrawQuad(const glm::vec2& position = { 0.0f, 0.0f }, float rotation = 0.0f, const glm::vec2& scale = { 1.0f, 1.0f }, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture);
	};
}

#pragma once

#include <glm/glm.hpp>

#include "Element/Core/Core.h"
#include "Element/Renderer/Texture.h"

namespace Element {
	class SubTexture2D {
	private:
		Ref<Texture2D> texture;

		glm::vec2 texCoords[4];
		
	public:
		SubTexture2D(const Ref<Texture2D>& texture,const glm::vec2& min, const glm::vec2& max);

	public:
		const Ref<Texture2D>& GetTexture() const {
			return texture;
		}

		const glm::vec2* GetTexCoords() const {
			return texCoords;
		}

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	};
}

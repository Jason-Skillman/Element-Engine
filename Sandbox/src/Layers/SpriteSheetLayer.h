#pragma once

#include <Element.h>

#include "ParticleSystem.h"

class SpriteSheetLayer : public Element::Layer {
private:
	Element::OrthographicCameraController cameraController;

	Element::Ref<Element::Texture2D> textureRPGSpriteSheet;
	Element::Ref<Element::SubTexture2D> texStairsLeft, texBarrel, texTreeOrange;

	uint32_t mapWidth = 24, mapHeight = 14;
	std::unordered_map<char, Element::Ref<Element::SubTexture2D>> textureMap;

public:
	SpriteSheetLayer();
	virtual ~SpriteSheetLayer() = default;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Element::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Element::Event& event) override;
};

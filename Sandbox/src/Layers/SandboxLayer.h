#pragma once

#include <Element.h>

#include "ParticleSystem.h"

struct ProfileResult {
	const char* name;
	float time;
};

class SandboxLayer : public Element::Layer {
private:
	Element::OrthographicCameraController cameraController;

	//Todo: Temp
	Element::Ref<Element::VertexArray> squareVA;
	Element::Ref<Element::Texture2D> textureCheckerboard, textureArrow, textureRPGSpriteSheet;
	Element::Ref<Element::SubTexture2D> texStairsLeft, texBarrel, texTreeOrange;
	
	glm::vec4 squareColor = { 0.2f, 0.2f, 0.8f, 1.0f };

	std::vector<ProfileResult> profileResults;

	ParticleSystem particleSystem;
	ParticleProps particleProps;

	uint32_t mapWidth = 24, mapHeight = 14;
	std::unordered_map<char, Element::Ref<Element::SubTexture2D>> textureMap;
	
public:
	SandboxLayer();
	virtual  ~SandboxLayer() = default;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Element::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Element::Event& event) override;
};

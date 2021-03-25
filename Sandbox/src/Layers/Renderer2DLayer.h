#pragma once

#include <Element.h>

#include "ParticleSystem.h"

class Renderer2DLayer : public Element::Layer {
private:
	Element::OrthographicCameraController cameraController;

	Element::Ref<Element::Texture2D> textureCheckerboard, textureArrow;

	ParticleSystem particleSystem;
	ParticleProps particleProps;

public:
	Renderer2DLayer();
	virtual ~Renderer2DLayer() = default;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Element::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Element::Event& event) override;
};

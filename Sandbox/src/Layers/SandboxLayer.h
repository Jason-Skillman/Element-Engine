#pragma once

#include <Element.h>

#include "ParticleSystem.h"

class SandboxLayer : public Element::Layer {
private:
	Element::OrthographicCameraController cameraController;

	Element::Ref<Element::Texture2D> textureCheckerboard, textureArrow;
	Element::Ref<Element::FrameBuffer> frameBuffer;
	
	ParticleSystem particleSystem;
	ParticleProps particleProps;
	
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Element::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Element::Event& event) override;
};

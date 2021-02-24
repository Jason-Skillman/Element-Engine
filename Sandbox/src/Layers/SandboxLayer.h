#pragma once

#include <Hazel.h>

#include "Timer.h"

struct ProfileResult {
	const char* name;
	float time;
};

class SandboxLayer : public Hazel::Layer {
private:
	Hazel::OrthographicCameraController cameraController;

	//Todo: Temp
	Hazel::Ref<Hazel::VertexArray> squareVA;
	//Hazel::Ref<Hazel::Shader> colorShader;
	Hazel::Ref<Hazel::Texture2D> texture;
	
	glm::vec4 squareColor = { 0.2f, 0.2f, 0.8f, 1.0f };

	std::vector<ProfileResult> profileResults;
	
public:
	SandboxLayer();
	virtual  ~SandboxLayer() = default;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;
};

#pragma once

#include "Hazel/Layer.h"
#include "Hazel/Log.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		HZ_INFO("ExampleLayer::Update()")
	}

	void OnEvent(Hazel::Event& event) override {
		HZ_TRACE("{0}", event)
	}
};

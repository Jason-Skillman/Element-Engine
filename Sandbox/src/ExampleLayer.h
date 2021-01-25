#pragma once

#include "Hazel/Layer.h"
#include "Hazel/Log.h"

#include "imgui.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		//HZ_PRINT_INFO("ExampleLayer::Update()")

		//Ex. Input key press
		if(Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) {
			HZ_PRINT_TRACE("Tab pressed");
		}
	}

	void OnImGuiRender() override {
		//ImGui::Begin("Test");
		//ImGui::Text("Hello");
		//ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override {
		//HZ_PRINT_TRACE("{0}", event)
	}
};

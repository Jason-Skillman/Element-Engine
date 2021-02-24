#include "SandboxLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <chrono>
#include <iostream>
#include <functional>

//#include "Timer.h"

template<typename Fn>
class Timer {
private:
	const char* name;
	Fn func;
	std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
	bool isStopped;

public:
	Timer(const char* name, Fn&& func)
		: name(name), func(func), isStopped(false) {

		startTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		if(!isStopped)
			Stop();
	}

public:
	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		isStopped = true;

		float duration = (end - start) * 0.001f;

		func({ name, duration });
	}
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { profileResults.push_back(profileResult); })

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), cameraController(1280.0f / 720.0f) { }

void SandboxLayer::OnAttach() {
	texture = Hazel::Texture2D::Create("assets/textures/cherno_checkerboard.png");
}

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate(Hazel::Timestep ts) {
	PROFILE_SCOPE("Sandbox::OnUpdate");
	
	//Update
	{
		PROFILE_SCOPE("Update");
		
		cameraController.OnUpdate(ts);
	}

	//Pre-render
	{
		PROFILE_SCOPE("Pre-render");
		
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	//Rendering
	{
		PROFILE_SCOPE("Rendering");

		Hazel::Renderer2D::BeginScene(cameraController.GetCamera());

		//Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f}, 0.0f, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, texture);

		Hazel::Renderer2D::DrawQuad({ -0.5f, -0.5f, 1.0 }, 15.0f, { 0.8f, 0.5f }, { 0.2f, 0.8f, 0.3f, 1.0f });

		Hazel::Renderer2D::EndScene();
	}
}

void SandboxLayer::OnImGuiRender() {
	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();

	ImGui::Begin("Profiler");

	for(auto& result : profileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	profileResults.clear();

	ImGui::End();
}

void SandboxLayer::OnEvent(Hazel::Event& event) {
	cameraController.OnEvent(event);
}

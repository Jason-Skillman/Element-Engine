#include "pch.h"
#include "OrthographicCameraController.h"

#include "Element/Codes/KeyCodes.h"
#include "Element/Core/Input.h"

namespace Element {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: aspectRatio(aspectRatio),
	bounds({ -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel }),
	camera(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top), cameraPosition(0), cameraRotation(0) {}
	
	void OrthographicCameraController::OnUpdate(Timestep ts) {
		//Move camera
		if(Input::IsKeyPressed(KEY_W)) cameraPosition.y += moveSpeed * zoomLevel * ts;
		else if(Input::IsKeyPressed(KEY_S)) cameraPosition.y -= moveSpeed * zoomLevel * ts;
		if(Input::IsKeyPressed(KEY_D)) cameraPosition.x += moveSpeed * zoomLevel * ts;
		else if(Input::IsKeyPressed(KEY_A)) cameraPosition.x -= moveSpeed * zoomLevel * ts;
		
		if(Input::IsKeyPressed(KEY_E)) cameraRotation -= rotationSpeed * ts;
		else if(Input::IsKeyPressed(KEY_Q)) cameraRotation += rotationSpeed * ts;

		camera.SetPosition(cameraPosition);
		camera.SetRotation(cameraRotation);
	}
	
	void OrthographicCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height) {
		aspectRatio = width / height;
		CalculateView();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		zoomLevel -= event.GetYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		
		CalculateView();
		return false;
	}
	
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event) {
		OnResize(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));
		return false;
	}

	void OrthographicCameraController::CalculateView() {
		bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel };
		camera.SetProjection(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top);
	}
}

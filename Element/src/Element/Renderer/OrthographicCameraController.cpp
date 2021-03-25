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
		if(Input::IsKeyPressed(KEY_W)) cameraPosition.y += cameraSpeed * ts;
		else if(Input::IsKeyPressed(KEY_S)) cameraPosition.y -= cameraSpeed * ts;
		if(Input::IsKeyPressed(KEY_D)) cameraPosition.x += cameraSpeed * ts;
		else if(Input::IsKeyPressed(KEY_A)) cameraPosition.x -= cameraSpeed * ts;
		
		if(Input::IsKeyPressed(KEY_E)) cameraRotation -= cameraRotSpeed * ts;
		else if(Input::IsKeyPressed(KEY_Q)) cameraRotation += cameraRotSpeed * ts;

		camera.SetPosition(cameraPosition);
		camera.SetRotation(cameraRotation);
	}
	
	void OrthographicCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}
	
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		zoomLevel -= event.GetYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		
		CalculateView();
		return false;
	}
	
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event) {
		aspectRatio = static_cast<float>(event.GetWidth()) / static_cast<float>(event.GetHeight());
		
		CalculateView();
		return false;
	}

	void OrthographicCameraController::CalculateView() {
		bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel };
		camera.SetProjection(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top);
	}
}

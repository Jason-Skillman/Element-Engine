#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Input.h"
#include "Codes/KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), cameraPosition(0), cameraRotation(0) {}
	
	void OrthographicCameraController::OnUpdate(Timestep ts) {
		//Move camera
		if(Input::IsKeyPressed(HZ_KEY_W)) cameraPosition.y += cameraSpeed * ts;
		else if(Input::IsKeyPressed(HZ_KEY_S)) cameraPosition.y -= cameraSpeed * ts;
		if(Input::IsKeyPressed(HZ_KEY_D)) cameraPosition.x += cameraSpeed * ts;
		else if(Input::IsKeyPressed(HZ_KEY_A)) cameraPosition.x -= cameraSpeed * ts;
		
		if(Input::IsKeyPressed(HZ_KEY_E)) cameraRotation -= cameraRotSpeed * ts;
		else if(Input::IsKeyPressed(HZ_KEY_Q)) cameraRotation += cameraRotSpeed * ts;

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

		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
	
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event) {
		aspectRatio = static_cast<float>(event.GetWidth()) / static_cast<float>(event.GetHeight());
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
}

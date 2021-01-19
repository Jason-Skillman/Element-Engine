#pragma once

#include "Hazel/Window.h"

struct GLFWwindow;

namespace Hazel {
	class WindowsWindow : public Window {
	private:
		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vsync;
			EventCallbackFunc eventCallback;
		};
		
	private:
		GLFWwindow* window;
		WindowData data;
		
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

	public:
		void OnUpdate() override;
		
		unsigned int GetWidth() override;
		unsigned int GetHeight() override;

		//Window attributes
		void SetEventCallback(const EventCallbackFunc& callback) override;
		void SetVsync(bool enabled) override;
		bool IsVsync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	};
}

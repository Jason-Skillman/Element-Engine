#pragma once

#include "Element/Core/Window.h"
#include "Element/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Element {

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
		GraphicsContext* context;
		
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

	public:
		virtual void OnUpdate() override;
		
		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFunc& callback) override;
		virtual void SetVsync(bool enabled) override;
		virtual bool IsVsync() const override;

		virtual void SetWindowIcon(const std::string& filepath) override;

		virtual void* GetNativeWindow() const override {
			return window;
		}
		
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	};
}

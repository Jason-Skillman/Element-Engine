#pragma once

#include "Hazel/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

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
		GraphicsContext* context;
		
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

	public:
		void OnUpdate() override;
		
		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		//Window attributes
		void SetEventCallback(const EventCallbackFunc& callback) override;
		void SetVsync(bool enabled) override;
		bool IsVsync() const override;

		inline void* GetNativeWindow() const override {
			return window;
		}
		
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	};
}

#pragma once
#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	struct WindowProps {
		std::string title;
		unsigned int width, height;

		WindowProps(const std::string& title = "Hazel Engine", unsigned int width = 1280, unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	//Interface representing a desktop system based Window
	class Window {
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() = default;
		
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVsync(bool enables) = 0;
		virtual bool IsVsync() const = 0;

		/// <summary>
		/// Returns a GLFW window
		/// </summary>
		/// <returns></returns>
		inline virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}

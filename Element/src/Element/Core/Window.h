#pragma once
#include "pch.h"

#include "Element/Events/Event.h"
#include "Element/Core/Resolution.h"

namespace Element {

	struct WindowProps {
		std::string title;
		uint32_t width, height;

		WindowProps(const std::string& title, uint32_t width, uint32_t height)
			: title(title), width(width), height(height) {}

		WindowProps(const std::string& title, Resolution resolution)
			: title(title), width(resolution.width), height(resolution.height) {}
	};

	/// <summary>
	/// Interface representing a desktop system based Window
	/// </summary>
	class Window {
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() = default;
		
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVsync(bool enables) = 0;
		virtual bool IsVsync() const = 0;

		/// <summary>
		/// Returns a GLFW window
		/// </summary>
		inline virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props);
	};
}

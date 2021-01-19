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
	class HAZEL_API Window {
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() = default;
		
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVsync(bool enables) = 0;
		virtual bool IsVsync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}

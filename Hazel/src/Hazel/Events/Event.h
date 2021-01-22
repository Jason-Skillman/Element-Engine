#pragma once

#include "Hazel/Core.h"

namespace Hazel {

	/*
	Events in Hazel are currently blocking, meaning when an event occurs it
	immediately gets dispatched and must be dealt with right then an there.
	For the future, a better strategy might be to buffer events in an event
	bus and process them during the "event" part of the update stage.
	*/

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	/// <summary>
	/// Base class for other events
	/// </summary>
	class HAZEL_API Event {
		friend class EventDispatcher;

	protected:
		bool handled = false;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		
		virtual std::string ToString() const {
			return GetName();
		}

		bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

		bool IsHandled() {
			return handled;
		}
	};
	
	class EventDispatcher {
		template<typename T>
		using EventFunc = std::function<bool(T&)>;

	private:
		Event& event;

	public:
		EventDispatcher(Event& event) : event(event) {}

	public:
		template<typename T>
		bool Dispatch(EventFunc<T> func) {
			if(event.GetEventType() == T::GetStaticType()) {
				event.handled = func(*(T*)&event);
				return true;
			}
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}

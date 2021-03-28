#pragma once

#include "Element/Core/Timestep.h"
#include "Element/Events/Event.h"

namespace Element {
	class Layer {
	protected:
		std::string debugName;

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

	public:
		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(Timestep ts);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& event);

		inline const std::string& GetName() const {
			return debugName;
		}
	};
}

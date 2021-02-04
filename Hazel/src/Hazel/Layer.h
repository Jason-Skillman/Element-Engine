#pragma once

#include "Hazel/Events/Event.h"
#include "Core/Timestep.h"

namespace Hazel {
	class Layer {
	protected:
		std::string debugName;

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

	public:
		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(Timestep timestep);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& event);

		inline const std::string& GetName() const {
			return debugName;
		}
	};
}

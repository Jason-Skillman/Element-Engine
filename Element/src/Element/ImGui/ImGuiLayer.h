#pragma once

#include "Element/Core/Layer.h"

namespace Element {
	class ImGuiLayer : public Layer {
	private:
		float deltaTime = 0.0f;
		
	public:
		ImGuiLayer();
		~ImGuiLayer();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};
}

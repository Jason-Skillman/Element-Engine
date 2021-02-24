#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel {
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

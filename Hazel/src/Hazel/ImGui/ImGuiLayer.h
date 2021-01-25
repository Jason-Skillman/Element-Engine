#pragma once
#include "Hazel/Layer.h"

namespace Hazel {
	class HAZEL_API ImGuiLayer : public Layer {
	private:
		float deltaTime = 0.0f;
		
	public:
		ImGuiLayer();
		~ImGuiLayer();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	};
}

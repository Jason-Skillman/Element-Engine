#pragma once

#include "Element/Core/Layer.h"

namespace Element {
	class ImGuiLayer : public Layer {
	private:
		bool blockEvents = true;
		
	public:
		ImGuiLayer();
		~ImGuiLayer();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetBlockEvents(bool value) {
			blockEvents = value;
		}

		void SetDarkThemeColors();
	};
}

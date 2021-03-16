#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel {
	class LayerStack {
	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertIndex = 0;

	public:
		LayerStack();
		~LayerStack();

	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator begin() {
			return layers.begin();
		}
		
		inline std::vector<Layer*>::iterator end() {
			return layers.end();
		}
	};
}

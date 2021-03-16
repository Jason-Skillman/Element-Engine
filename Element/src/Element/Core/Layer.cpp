#include "pch.h"
#include "Layer.h"

namespace Element {
	Layer::Layer(const std::string& name)
		: debugName(name) {}

	Layer::~Layer() {}

	void Layer::OnAttach() {}

	void Layer::OnDetach() {}

	void Layer::OnUpdate(Timestep ts) {}
	
	void Layer::OnImGuiRender() {}

	void Layer::OnEvent(Event& event) {}
}

#include "hzpch.h"
#include "Layer.h"

namespace Hazel {
	Layer::Layer(const std::string& name)
		: debugName(name) {}

	Layer::~Layer() {}

	void Layer::OnAttach() {}

	void Layer::OnDetach() {}

	void Layer::OnUpdate() {}

	void Layer::OnEvent(Event& event) {}

	const std::string& Hazel::Layer::GetName() const {
		return debugName;
	}
}

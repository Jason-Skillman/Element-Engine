#pragma once

namespace Element {
	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;
		
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}

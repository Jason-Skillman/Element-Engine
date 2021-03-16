#include "pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Element {

	RendererAPI* RenderCommand::rendererApi = new OpenGLRendererAPI();
}

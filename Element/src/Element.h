/*
Intended to be included by other client applications.
*/

#pragma once

//Codes
#include "Element/Codes/KeyCodes.h"
#include "Element/Codes/MouseCodes.h"

//Core
#include "Element/Core/Core.h"
#include "Element/Core/Application.h"
#include "Element/Core/Layer.h"
#include "Element/Core/Log.h"
#include "Element/Core/Input.h"
#include "Element/Core/Timestep.h"

//Debug
#include "Element/Debug/Instrumentor.h"

//Events
#include "Element/Events/Event.h"
#include "Element/Events/ApplicationEvent.h"
#include "Element/Events/MouseEvent.h"
#include "Element/Events/KeyEvent.h"

//ImGui
#include "Element/ImGui/ImGuiLayer.h"

//Math
#include "Element/Math/Random.h"

//Renderer
#include "Element/Renderer/Renderer.h"
#include "Element/Renderer/Renderer2D.h"
#include "Element/Renderer/RenderCommand.h"
#include "Element/Renderer/OrthographicCamera.h"
#include "Element/Renderer/OrthographicCameraController.h"
#include "Element/Renderer/Buffer.h"
#include "Element/Renderer/VertexArray.h"
#include "Element/Renderer/Shader.h"
#include "Element/Renderer/Texture.h"
#include "Element/Renderer/FrameBuffer.h"

/*
Intended to be included by other client applications.
*/

#pragma once

//Codes
#include "Hazel/Codes/KeyCodes.h"
#include "Hazel/Codes/MouseCodes.h"

//Core
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Application.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Timestep.h"

//Events
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

//ImGui
#include "Hazel/ImGui/ImGuiLayer.h"

//Renderer
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/OrthographicCameraController.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"

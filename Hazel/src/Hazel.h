/*
Intended to be included by other client applications.
*/

#pragma once

#include "Hazel/Application.h"
#include "Hazel/Layer.h"
#include "Hazel/Log.h"
#include "Hazel/Input.h"

//Codes
#include "Hazel/Codes/KeyCodes.h"
#include "Hazel/Codes/MouseCodes.h"

//ImGui
#include "Hazel/ImGui/ImGuiLayer.h"

//Renderer
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"

//Entry Point
#include "Hazel/EntryPoint.h"

#pragma once

#include "Application.h"
#include "Log.h"
#include "Element/Math/Random.h"

#ifdef PLATFORM_WINDOWS

extern Element::Application* Element::CreateApplication();

int main(int argc, char** argv) {
	EL_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	Element::Log::Init();
	Element::Random::Init();
	Element::Application* app = Element::CreateApplication();
	EL_PROFILE_END_SESSION();
	
	EL_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->Run();
	EL_PROFILE_END_SESSION();

	EL_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
	delete app;
	EL_PROFILE_END_SESSION();
	
	return 0;
}

#else
	#error Element Engine only supports Windows
#endif

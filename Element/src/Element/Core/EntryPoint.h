#pragma once

#include "Application.h"
#include "Log.h"

#ifdef PLATFORM_WINDOWS

extern Element::Application* Element::CreateApplication();

int main(int argc, char** argv) {
	PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	Element::Log::Init();
	Element::Random::Init();
	Element::Application* app = Element::CreateApplication();
	PROFILE_END_SESSION();
	
	PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
	delete app;
	PROFILE_END_SESSION();
	
	return 0;
}

#else
	#error Element Engine only supports Windows
#endif

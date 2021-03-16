#pragma once

#include "Application.h"
#include "Log.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Element::Application* Element::CreateApplication();

int main(int argc, char** argv) {
	HZ_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	Element::Log::Init();
	Element::Random::Init();
	Element::Application* app = Element::CreateApplication();
	HZ_PROFILE_END_SESSION();
	
	HZ_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
	
	return 0;
}

#else
	#error Element Engine only supports Windows
#endif

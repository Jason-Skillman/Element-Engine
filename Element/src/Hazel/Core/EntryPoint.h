#pragma once

#include "Application.h"
#include "Log.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	Hazel::Log::Init();
	Hazel::Random::Init();
	Hazel::Application* app = Hazel::CreateApplication();
	HZ_PROFILE_END_SESSION();
	
	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
	
	return 0;
}

#else
	#error Hazel Engine only supports Windows
#endif

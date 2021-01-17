#pragma once

#include "Application.h"
#include "Log.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	HZ_INFO("Initialized Log!");
	Hazel::Log::GetCoreLogger()->info("");
	
	auto app = Hazel::CreateApplication();
	app->Run();

	delete app;

	return 0;
}

#else
	#error Hazel Engine only supports Windows
#endif

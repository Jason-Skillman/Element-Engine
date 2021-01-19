#pragma once

#include "Application.h"
#include "Log.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	Hazel::Log::Init();
	HZ_CORE_WARN("Hazel Engine");
	HZ_CORE_TRACE("Initialized Log!");
	
	auto app = Hazel::CreateApplication();
	app->Run();

	delete app;

	return 0;
}

#else
	#error Hazel Engine only supports Windows
#endif

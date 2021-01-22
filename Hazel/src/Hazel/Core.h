#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif
#else
	#error Hazel Engine only supports Windows
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

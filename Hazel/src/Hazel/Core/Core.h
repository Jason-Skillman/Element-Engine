#pragma once

#include <memory>

//Windows DLL
#ifdef HZ_PLATFORM_WINDOWS

	//HAZEL_API is depreciated
	#if HZ_DYNAMIC_LINK
		#ifdef HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)
		#else
			#define HAZEL_API __declspec(dllimport)
		#endif
	#else
		#define HAZEL_API
	#endif

#else
	#error Hazel Engine only supports Windows
#endif


//Asserts
#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS 1
#endif

#if HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(x) { HZ_ERROR(__VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(x) { HZ_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

namespace Hazel {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}

#pragma once

#include <memory>

//Windows DLL
#ifdef PLATFORM_WINDOWS

	//ELEMENT_API is depreciated
	#if DYNAMIC_LINK
		#ifdef BUILD_DLL
			#define ELEMENT_API __declspec(dllexport)
		#else
			#define ELEMENT_API __declspec(dllimport)
		#endif
	#else
		#define ELEMENT_API
	#endif

#else
	#error Element Engine only supports Windows
#endif


//Asserts
#ifdef DEBUG
	#define ENABLE_ASSERTS 1
#endif

#if ENABLE_ASSERTS
	#define ASSERT(x, ...) { if(x) { LOG_ERROR(__VA_ARGS__); __debugbreak(); } }
	#define CORE_ASSERT(x, ...) { if(x) { LOG_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
	#define ASSERT(x, ...)
	#define CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

namespace Element {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

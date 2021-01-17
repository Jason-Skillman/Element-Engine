#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

//Core logging macros
#define HZ_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define HZ_CORE_DEBUG(...) ::Hazel::Log::GetCoreLogger()->debug(__VA_ARGS__);
#define HZ_CORE_INFO(...) ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__);
#define HZ_CORE_WARN(...) ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define HZ_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__);
#define HZ_CORE_CRITICAL(...) ::Hazel::Log::GetCoreLogger()->critical(__VA_ARGS__);

//Client logging macros
#define HZ_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__);
#define HZ_DEBUG(...) ::Hazel::Log::GetClientLogger()->debug(__VA_ARGS__);
#define HZ_INFO(...) ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__);
#define HZ_WARN(...) ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__);
#define HZ_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__);
#define HZ_CRITICAL(...) ::Hazel::Log::GetClientLogger()->critical(__VA_ARGS__);

namespace Hazel {
	class HAZEL_API Log {
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
		
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger();
	};
}

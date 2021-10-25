#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

//Core logging macros
#define EL_LOG_CORE_TRACE(...) ::Element::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define EL_LOG_CORE_DEBUG(...) ::Element::Log::GetCoreLogger()->debug(__VA_ARGS__);
#define EL_LOG_CORE_INFO(...) ::Element::Log::GetCoreLogger()->info(__VA_ARGS__);
#define EL_LOG_CORE_WARN(...) ::Element::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define EL_LOG_CORE_ERROR(...) ::Element::Log::GetCoreLogger()->error(__VA_ARGS__);
#define EL_LOG_CORE_CRITICAL(...) ::Element::Log::GetCoreLogger()->critical(__VA_ARGS__);

//Client logging macros
#define EL_LOG_TRACE(...) ::Element::Log::GetClientLogger()->trace(__VA_ARGS__);
#define EL_LOG_DEBUG(...) ::Element::Log::GetClientLogger()->debug(__VA_ARGS__);
#define EL_LOG_INFO(...) ::Element::Log::GetClientLogger()->info(__VA_ARGS__);
#define EL_LOG_WARN(...) ::Element::Log::GetClientLogger()->warn(__VA_ARGS__);
#define EL_LOG_ERROR(...) ::Element::Log::GetClientLogger()->error(__VA_ARGS__);
#define EL_LOG_CRITICAL(...) ::Element::Log::GetClientLogger()->critical(__VA_ARGS__);

namespace Element {
	class Log {
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
		
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return coreLogger;
		}

		static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return clientLogger;
		}
	};
}

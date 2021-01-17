#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {

	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;
	
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		coreLogger = spdlog::stdout_color_mt("HAZEL");
		coreLogger->set_level(spdlog::level::trace);
		
		clientLogger = spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);
	}
	
	std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() {
		return coreLogger;
	}
	
	std::shared_ptr<spdlog::logger>& Log::GetClientLogger() {
		return clientLogger;
	}
}

#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Becketron {

	class BTRON_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
// Core Log macros
#define BT_CORE_TRACE(...)	::Becketron::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define BT_CORE_INFO(...)	::Becketron::Log::GetCoreLogger()->info(__VA_ARGS__);
#define BT_CORE_WARN(...)	::Becketron::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define BT_CORE_ERROR(...)	::Becketron::Log::GetCoreLogger()->error(__VA_ARGS__);
#define BT_CORE_FATAL(...)	::Becketron::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// Client Log macros
#define BT_TRACE(...)	::Becketron::Log::GetClientLogger()->trace(__VA_ARGS__);
#define BT_INFO(...)	::Becketron::Log::GetClientLogger()->info(__VA_ARGS__);
#define BT_WARN(...)	::Becketron::Log::GetClientLogger()->warn(__VA_ARGS__);
#define BT_ERROR(...)	::Becketron::Log::GetClientLogger()->error(__VA_ARGS__);
#define BT_FATAL(...)	::Becketron::Log::GetClientLogger()->fatal(__VA_ARGS__);

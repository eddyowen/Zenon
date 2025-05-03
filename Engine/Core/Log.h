#pragma once

#include "Core/Base.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

namespace zn
{
	class Log
	{
	public:
		static void Init();

		static SharedPtr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }

	private:
		static SharedPtr<spdlog::logger> s_coreLogger;
	};
}

#ifdef ZN_DEBUG
	#define ZN_CORE_TRACE(...)    ::zn::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define ZN_CORE_INFO(...)     ::zn::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define ZN_CORE_WARN(...)     ::zn::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define ZN_CORE_ERROR(...)    ::zn::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define ZN_CORE_CRITICAL(...) ::zn::Log::GetCoreLogger()->critical(__VA_ARGS__)

	#define ZN_CORE_FLUSH_LOG()   ::zn::Log::GetCoreLogger()->flush()
#else
	#define ZN_CORE_INFO(...)    
	#define ZN_CORE_TRACE(...)   
	#define ZN_CORE_WARN(...)    
	#define ZN_CORE_ERROR(...)   
	#define ZN_CORE_CRITICAL(...)
#endif
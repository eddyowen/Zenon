#include "Log.h"

#pragma warning(push, 0)
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning(pop)

namespace zn
{
	SharedPtr<spdlog::logger> Log::s_CoreLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = CreateShared<spdlog::logger>("LogCore", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);
	}
}


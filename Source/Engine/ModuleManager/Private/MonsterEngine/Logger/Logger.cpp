#include "MonsterEngine/Logger/Logger.h"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace
{
	static std::shared_ptr<spdlog::logger> s_Logger = nullptr;
}

namespace MonsterEngine::Logger
{
	std::shared_ptr<spdlog::logger> GetLogger()
	{
		if (!s_Logger)
		{
			std::vector<spdlog::sink_ptr> sinks;
			if constexpr (s_IsConfigDebug)
				sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("Logs/Log", 23, 59));
			s_Logger = std::make_shared<spdlog::logger>("MonsterEngine", sinks.begin(), sinks.end());
			spdlog::set_pattern("[%%.%f][%^%8l%$][%7t] %v");

			if constexpr (s_IsConfigDist)
				s_Logger->set_level(spdlog::level::level_enum::err);
			else
				s_Logger->set_level(spdlog::level::level_enum::trace);
		}
		return s_Logger;
	}
} // namespace MonsterEngine::Logger
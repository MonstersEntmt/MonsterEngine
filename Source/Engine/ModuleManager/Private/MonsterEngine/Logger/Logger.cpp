#include "MonsterEngine/Logger/Logger.h"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace MonsterEngine::Logger
{
	static std::vector<spdlog::sink_ptr> s_SharedSinks;

	static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& category)
	{
		if (s_SharedSinks.empty())
		{
			if constexpr (s_IsConfigDebug)
				s_SharedSinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			s_SharedSinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("Logs/Log", 23, 59));
		}

		auto logger = std::make_shared<spdlog::logger>(category, s_SharedSinks.begin(), s_SharedSinks.end());
		logger->set_pattern("[%%.%f][%^%8l%$][%7t] %v");

		if constexpr (s_IsConfigDist)
			logger->set_level(spdlog::level::level_enum::err);
		else
			logger->set_level(spdlog::level::level_enum::trace);
		return logger;
	}

	std::shared_ptr<spdlog::logger> GetLogger(const std::string& category)
	{
		auto logger = spdlog::get(category);

		if (!logger)
		{
			logger = CreateLogger(category);
			spdlog::register_logger(logger);
		}

		return logger;
	}
} // namespace MonsterEngine::Logger
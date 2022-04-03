#include "MonsterEngine/Logger/Logger.h"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace MonsterEngine
{
	static std::vector<spdlog::sink_ptr> s_SharedSinks;

	static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& category)
	{
		std::string name = category.substr(0, std::min<std::uint64_t>(category.size(), 16ULL));

		if (s_SharedSinks.empty())
		{
			if constexpr (s_IsConfigDebug)
				s_SharedSinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			s_SharedSinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("Logs/Log", 23, 59));
		}

		auto logger = std::make_shared<spdlog::logger>(name, s_SharedSinks.begin(), s_SharedSinks.end());
		logger->set_pattern("[%T.%f][%16n][%^%8l%$][%7t] %v");

		if constexpr (s_IsConfigDist)
			logger->set_level(spdlog::level::level_enum::err);
		else
			logger->set_level(spdlog::level::level_enum::trace);
		return logger;
	}

	Logger::Logger(const std::string& category)
	    : m_Logger(spdlog::get(category))
	{
		if (!m_Logger)
		{
			m_Logger = CreateLogger(category);
			spdlog::register_logger(m_Logger);
		}
	}
} // namespace MonsterEngine
#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace MonsterEngine::Logger
{
	MonsterEngine_ModuleManager_API std::shared_ptr<spdlog::logger> GetLogger();

	template <class... Args>
	void Trace(Args&&... args)
	{
		GetLogger()->trace(std::forward<Args>(args)...);
	}

	template <class... Args>
	void Info(Args&&... args)
	{
		GetLogger()->info(std::forward<Args>(args)...);
	}

	template <class... Args>
	void Debug(Args&&... args)
	{
		GetLogger()->debug(std::forward<Args>(args)...);
	}

	template <class... Args>
	void Warn(Args&&... args)
	{
		GetLogger()->warn(std::forward<Args>(args)...);
	}

	template <class... Args>
	void Error(Args&&... args)
	{
		GetLogger()->error(std::forward<Args>(args)...);
	}

	template <class... Args>
	void Critical(Args&&... args)
	{
		GetLogger()->critical(std::forward<Args>(args)...);
	}
} // namespace MonsterEngine::Logger
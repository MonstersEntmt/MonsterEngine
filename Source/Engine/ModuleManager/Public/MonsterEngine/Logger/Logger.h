#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace MonsterEngine::Logger
{
	MonsterEngine_ModuleManager_API std::shared_ptr<spdlog::logger> GetLogger(const std::string& category = "MonsterEngine");

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

	template <class... Args>
	void CatTrace(const std::string& category, Args&&... args)
	{
		GetLogger(category)->trace(std::forward<Args>(args)...);
	}

	template <class... Args>
	void CatInfo(const std::string& category, Args&&... args)
	{
		GetLogger(category)->info(std::forward<Args>(args)...);
	}

	template <class... Args>
	void CatDebug(const std::string& category, Args&&... args)
	{
		GetLogger(category)->debug(std::forward<Args>(args)...);
	}

	template <class... Args>
	void CatWarn(const std::string& category, Args&&... args)
	{
		GetLogger(category)->warn(std::forward<Args>(args)...);
	}

	template <class... Args>
	void CatError(const std::string& category, Args&&... args)
	{
		GetLogger(category)->error(std::forward<Args>(args)...);
	}

	template <class... Args>
	void CatCritical(const std::string& category, Args&&... args)
	{
		GetLogger(category)->critical(std::forward<Args>(args)...);
	}
} // namespace MonsterEngine::Logger
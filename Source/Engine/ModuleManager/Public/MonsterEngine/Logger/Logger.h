#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>

#if __cpp_lib_source_location
#include <source_location>
#endif

namespace MonsterEngine
{
	template <class T>
	concept NonFormatString = !spdlog::is_convertible_to_any_format_string<const T&>::value;

	class MonsterEngine_ModuleManager_API Logger
	{
	public:
		struct MonsterEngine_ModuleManager_API Src
		{
		public:
#if __cpp_lib_source_location
			Src(const std::source_location loc = std::source_location::current())
			    : m_Location(loc.file_name(), loc.line(), loc.function_name())
			{
			}
#else
			Src()
			{
			}
#endif

		public:
			spdlog::source_loc m_Location;
		};

	public:
		Logger(const std::string& category = "MonsterEngine");

		template <class... Args>
		void log(Src src, spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			m_Logger->log(src.m_Location, lvl, "[{}:{} {}] {}", src.m_Location.filename, src.m_Location.line, src.m_Location.funcname, fmt::format<Args...>(fmt, std::forward<Args>(args)...));
		}

		template <class... Args>
		void log(spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			m_Logger->log<Args...>(lvl, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void log(Src src, spdlog::level::level_enum lvl, const T& msg)
		{
			m_Logger->log(src.m_Location, lvl, "[{}:{} {}] {}", src.m_Location.filename, src.m_Location.line, src.m_Location.funcname, msg);
		}

		template <NonFormatString T>
		void log(spdlog::level::level_enum lvl, const T& msg)
		{
			m_Logger->log<T>(lvl, msg);
		}

		template <class... Args>
		void trace(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(src, spdlog::level::trace, fmt, std::forward<Args>(args)...);
		}

		template <class... Args>
		void trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(spdlog::level::trace, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void trace(Src src, const T& msg)
		{
			log<T>(src, spdlog::level::trace, msg);
		}

		template <NonFormatString T>
		void trace(const T& msg)
		{
			log<T>(spdlog::level::trace, msg);
		}

		template <class... Args>
		void debug(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(src, spdlog::level::debug, fmt, std::forward<Args>(args)...);
		}

		template <class... Args>
		void debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(spdlog::level::debug, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void debug(Src src, const T& msg)
		{
			log<T>(src, spdlog::level::debug, msg);
		}

		template <NonFormatString T>
		void debug(const T& msg)
		{
			log<T>(spdlog::level::debug, msg);
		}

		template <class... Args>
		void info(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(src, spdlog::level::info, fmt, std::forward<Args>(args)...);
		}

		template <class... Args>
		void info(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(spdlog::level::info, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void info(Src src, const T& msg)
		{
			log<T>(src, spdlog::level::info, msg);
		}

		template <NonFormatString T>
		void info(const T& msg)
		{
			log<T>(spdlog::level::info, msg);
		}

		template <class... Args>
		void warn(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(src, spdlog::level::warn, fmt, std::forward<Args>(args)...);
		}

		template <class... Args>
		void warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(spdlog::level::warn, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void warn(Src src, const T& msg)
		{
			log<T>(src, spdlog::level::warn, msg);
		}

		template <NonFormatString T>
		void warn(const T& msg)
		{
			log<T>(spdlog::level::warn, msg);
		}

		template <class... Args>
		void error(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(src, spdlog::level::err, fmt, std::forward<Args>(args)...);
		}

		template <class... Args>
		void error(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(spdlog::level::err, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void error(Src src, const T& msg)
		{
			log<T>(src, spdlog::level::err, msg);
		}

		template <NonFormatString T>
		void error(const T& msg)
		{
			log<T>(spdlog::level::err, msg);
		}

		template <class... Args>
		void critical(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(src, spdlog::level::critical, fmt, std::forward<Args>(args)...);
		}

		template <class... Args>
		void critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			log<Args...>(spdlog::level::critical, fmt, std::forward<Args>(args)...);
		}

		template <NonFormatString T>
		void critical(Src src, const T& msg)
		{
			log<T>(src, spdlog::level::critical, msg);
		}

		template <NonFormatString T>
		void critical(const T& msg)
		{
			log<T>(spdlog::level::critical, msg);
		}

		template <class... Args>
		void exception(Src src, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			critical<Args...>(src, fmt, std::forward<Args>(args)...);
			throw std::runtime_error("");
		}

		template <class... Args>
		void exception(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			critical<Args...>(fmt, std::forward<Args>(args)...);
			throw std::runtime_error("");
		}

		template <NonFormatString T>
		void exception(Src src, const T& msg)
		{
			critical<T>(src, msg);
			throw std::runtime_error("");
		}

		template <NonFormatString T>
		void exception(const T& msg)
		{
			critical<T>(msg);
			throw std::runtime_error("");
		}

	private:
		std::shared_ptr<spdlog::logger> m_Logger;
	};
} // namespace MonsterEngine
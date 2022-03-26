#pragma once

#include "Window.h"

#include <cstdint>

#include <map>
#include <random>
#include <string_view>

namespace MonsterEngine::WindowManager
{
	class MonsterEngine_WindowManager_API WindowManager
	{
	public:
		static WindowManager& Get();
		static void           Destroy();

	public:
		std::uint32_t createWindow(std::uint32_t width, std::uint32_t height, std::string_view title);
		void          destroyWindow(std::uint32_t window);

		bool update();

	private:
		WindowManager();
		WindowManager(const WindowManager&) = delete;
		WindowManager(WindowManager&&)      = delete;
		WindowManager& operator=(const WindowManager&) = delete;
		WindowManager& operator=(WindowManager&&) = delete;
		~WindowManager()                          = default;

		std::uint32_t newWindowID();

	private:
		std::mt19937 m_Rng;

		std::map<std::uint32_t, Window> m_Windows;
	};
} // namespace MonsterEngine::WindowManager
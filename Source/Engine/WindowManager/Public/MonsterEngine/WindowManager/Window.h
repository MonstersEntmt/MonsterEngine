#pragma once

#include <cstdint>

#include <string>

typedef struct GLFWwindow GLFWwindow;

namespace MonsterEngine::WindowManager
{
	class MonsterEngine_WindowManager_API Window
	{
	public:
		Window(std::uint32_t id, std::uint32_t width, std::uint32_t height, const std::string& title);
		Window(std::uint32_t id, std::uint32_t width, std::uint32_t height, std::string&& title);

		void create();
		void destroy();

		bool isCreated() const;
		bool shouldCloseWindow() const;

		auto  getId() const { return m_ID; }
		auto  getWidth() const { return m_Width; }
		auto  getHeight() const { return m_Height; }
		auto& getTitle() const { return m_Title; }
		auto  getNative() const { return m_Native; }

	private:
		std::uint32_t m_ID;

		std::uint32_t m_Width, m_Height;
		std::string   m_Title;

		GLFWwindow* m_Native = nullptr;
	};
} // namespace MonsterEngine::WindowManager
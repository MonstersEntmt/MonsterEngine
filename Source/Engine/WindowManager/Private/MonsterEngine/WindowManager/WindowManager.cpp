#include "MonsterEngine/WindowManager/WindowManager.h"

#include <MonsterEngine/Logger/Logger.h>

#include <GLFW/glfw3.h>

namespace MonsterEngine::WindowManager
{
	WindowManager& WindowManager::Get()
	{
		static WindowManager* s_Instance = new WindowManager();
		return *s_Instance;
	}

	void WindowManager::Destroy()
	{
		delete &Get();
	}

	Window* WindowManager::getWindow(std::uint32_t window)
	{
		auto itr = m_Windows.find(window);
		return itr != m_Windows.end() ? &itr->second : nullptr;
	}

	std::uint32_t WindowManager::createWindow(std::uint32_t width, std::uint32_t height, std::string_view title)
	{
		std::uint32_t id     = newWindowID();
		auto&         window = m_Windows.insert({ id, Window { id, width, height, std::string { title } } }).first->second;
		window.create();
		Logger::Trace("Created window '{}' at {}x{}", window.getTitle(), window.getWidth(), window.getHeight());
		return id;
	}

	void WindowManager::destroyWindow(std::uint32_t window)
	{
		auto itr = m_Windows.find(window);
		if (itr != m_Windows.end())
		{
			itr->second.destroy();
			Logger::Trace("Destroyed window '{}'", itr->second.getTitle());
			m_Windows.erase(itr);
		}
	}

	bool WindowManager::update()
	{
		glfwPollEvents();
		std::erase_if(m_Windows,
		              [](const std::pair<std::uint32_t, Window>& window) -> bool
		              {
			              if (window.second.shouldCloseWindow())
			              {
				              Logger::Trace("Destroyed window '{}'", window.second.getTitle());
				              return true;
			              }
			              return false;
		              });
		return !m_Windows.empty();
	}

	WindowManager::WindowManager()
	    : m_Rng(std::random_device {}())
	{
		if (!glfwInit())
			throw std::runtime_error("GLFW failed to initialize!");
	}

	WindowManager::~WindowManager()
	{
		glfwTerminate();
	}

	std::uint32_t WindowManager::newWindowID()
	{
		std::uint32_t id = m_Rng();
		while (id == 0 || m_Windows.find(id) != m_Windows.end())
			id = m_Rng();
		return id;
	}
} // namespace MonsterEngine::WindowManager
#include "MonsterEngine/WindowManager/Window.h"

#include <MonsterEngine/Logger/Logger.h>
#include <MonsterEngine/Renderer/RHI/IDevice.h>
#include <MonsterEngine/Renderer/RHI/IRHI.h>
#include <MonsterEngine/Renderer/RHI/Registry.h>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <utility>

namespace MonsterEngine::WindowManager
{
	Window::Window(std::uint32_t id, std::uint32_t width, std::uint32_t height, const std::string& title)
	    : m_ID(id), m_Width(width), m_Height(height), m_Title(title) {}

	Window::Window(std::uint32_t id, std::uint32_t width, std::uint32_t height, std::string&& title)
	    : m_ID(id), m_Width(width), m_Height(height), m_Title(std::move(title)) {}

	void Window::create()
	{
		if (m_Native)
			return;

		auto logger = Logger("WindowManager");

		auto rhi = Renderer::RHI::Registry::Get().getSelectedRHI();
		if (!rhi)
			logger.exception({}, "No RHI was selected prior to window creation!");
		glfwDefaultWindowHints();
		rhi->setGLFWOptions(*this);

		m_Native = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Native)
			logger.exception({}, "Failed to create window '{}'!", m_Title);

		m_Swapchain = rhi->getMainDevice()->newSwapchain(*this);
	}

	void Window::destroy()
	{
		if (!m_Native)
			return;

		m_Swapchain = nullptr;
		glfwDestroyWindow(m_Native);
		m_Native = nullptr;
	}

	bool Window::isCreated() const
	{
		return m_Native;
	}

	bool Window::shouldCloseWindow() const
	{
		return glfwWindowShouldClose(m_Native);
	}
} // namespace MonsterEngine::WindowManager
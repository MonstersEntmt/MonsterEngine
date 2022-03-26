#include "MonsterEngine/WindowManager/Window.h"

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

		auto rhi = Renderer::RHI::Registry::Get().getSelectedRHI();
		if (!rhi)
			throw std::runtime_error("No RHI was selected prior to window creation!");
		glfwDefaultWindowHints();
		rhi->setGLFWOptions(*this);

		m_Native = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Native)
			throw std::runtime_error("GLFW failed to create window!");
	}

	void Window::destroy()
	{
		if (!m_Native)
			return;

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
#include "MonsterEngine/Renderer/Vulkan/VulkanSurface.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"

#include <MonsterEngine/WindowManager/Window.h>
#include <MonsterEngine/WindowManager/WindowManager.h>

#include <GLFW/glfw3.h>

#include <utility>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanSurface::VulkanSurface(const std::string& name, VulkanDevice* device, WindowManager::Window& window)
	    : ISurface(name), m_Device(device), m_WindowId(window.getId())
	{
		create();
	}

	VulkanSurface::VulkanSurface(std::string&& name, VulkanDevice* device, WindowManager::Window& window)
	    : ISurface(std::move(name)), m_Device(device), m_WindowId(window.getId())
	{
		create();
	}

	VulkanSurface::~VulkanSurface()
	{
		VulkanSurface::destroy();
	}

	void VulkanSurface::create()
	{
		auto window = WindowManager::WindowManager::Get().getWindow(m_WindowId);
		if (!window)
			throw std::runtime_error("Invalid window used for vulkan surface!");

		if (glfwCreateWindowSurface(m_Device->getInstance()->getHandle(), window->getNative(), nullptr, &m_Surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create vulkan surface!");
	}

	void VulkanSurface::destroy()
	{
		if (m_Surface)
			vkDestroySurfaceKHR(m_Device->getInstance()->getHandle(), m_Surface, nullptr);
		m_Surface = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanSurface.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanDevice::VulkanDevice(VulkanInstance* instance, VkDevice device)
	    : m_Instance(instance), m_Device(device) {}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_Device, nullptr);
	}

	std::unique_ptr<RHI::ISurface> VulkanDevice::newSurface(WindowManager::Window& window)
	{
		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(m_Instance->getHandle(), window.getNative(), nullptr, &surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create vulkan surface!");
		return std::make_unique<VulkanSurface>(this, window, surface);
	}
} // namespace MonsterEngine::Renderer::Vulkan
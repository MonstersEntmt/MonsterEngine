#include "MonsterEngine/Renderer/Vulkan/VulkanSurface.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"

#include <MonsterEngine/WindowManager/Window.h>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanSurface::VulkanSurface(VulkanDevice* device, WindowManager::Window& window, VkSurfaceKHR surface)
	    : m_Device(device), m_WindowId(window.getId()), m_Surface(surface) {}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_Device->getInstance()->getHandle(), m_Surface, nullptr);
	}
} // namespace MonsterEngine::Renderer::Vulkan
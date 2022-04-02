#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanSurface.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <stdexcept>
#include <string>
#include <utility>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanDevice::VulkanDevice(const std::string& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice, VkDevice device)
	    : IDevice(name), m_Instance(instance), m_PhysicalDevice(physicalDevice), m_Device(device) {}

	VulkanDevice::VulkanDevice(std::string&& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice, VkDevice device)
	    : IDevice(std::move(name)), m_Instance(instance), m_PhysicalDevice(physicalDevice), m_Device(device) {}

	VulkanDevice::~VulkanDevice()
	{
		VulkanDevice::destroy();
	}

	std::unique_ptr<RHI::ISurface> VulkanDevice::newSurface(WindowManager::Window& window)
	{
		return std::make_unique<VulkanSurface>("Window " + std::to_string(window.getId()), this, window);
	}

	void VulkanDevice::destroy()
	{
		if (m_Device)
			vkDestroyDevice(m_Device, nullptr);
		m_PhysicalDevice = nullptr;
		m_Device         = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanHelper.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanSwapchain.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <stdexcept>
#include <string>
#include <utility>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanDevice::VulkanDevice(const std::string& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice)
	    : IDevice(name), m_Instance(instance), m_PhysicalDevice(physicalDevice)
	{
		create();
	}

	VulkanDevice::VulkanDevice(std::string&& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice)
	    : IDevice(std::move(name)), m_Instance(instance), m_PhysicalDevice(physicalDevice)
	{
		create();
	}

	VulkanDevice::~VulkanDevice()
	{
		VulkanDevice::destroy();
	}

	std::unique_ptr<RHI::ISwapchain> VulkanDevice::newSwapchain(WindowManager::Window& window)
	{
		return std::make_unique<VulkanSwapchain>("Window " + std::to_string(window.getId()), this, window);
	}

	void VulkanDevice::create()
	{
		VkPhysicalDeviceFeatures enabledFeatures = {};

		VkDeviceCreateInfo createInfo      = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext                   = nullptr;
		createInfo.flags                   = 0;
		createInfo.queueCreateInfoCount    = 0;
		createInfo.pQueueCreateInfos       = nullptr;
		createInfo.enabledLayerCount       = 0;
		createInfo.ppEnabledLayerNames     = nullptr;
		createInfo.enabledExtensionCount   = 0;
		createInfo.ppEnabledExtensionNames = nullptr;
		createInfo.pEnabledFeatures        = &enabledFeatures;

		VkCall({}, vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device),
		       "Failed to create VkDevice");
	}

	void VulkanDevice::destroy()
	{
		if (m_Device)
			vkDestroyDevice(m_Device, nullptr);
		m_PhysicalDevice = nullptr;
		m_Device         = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
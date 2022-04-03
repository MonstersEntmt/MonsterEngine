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
	    : IDevice(name, instance), m_PhysicalDevice(physicalDevice)
	{
		create();
	}

	VulkanDevice::VulkanDevice(std::string&& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice)
	    : IDevice(std::move(name), instance), m_PhysicalDevice(physicalDevice)
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

	VulkanInstance* VulkanDevice::getVulkanInstance() const
	{
		return static_cast<VulkanInstance*>(getInstance());
	}

	void VulkanDevice::create()
	{
		std::uint32_t graphicsIndex = 0;

		float priorities[] { 1.0f };

		VkDeviceQueueCreateInfo queueCreateInfo {};
		queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.pNext            = nullptr;
		queueCreateInfo.flags            = 0;
		queueCreateInfo.queueFamilyIndex = graphicsIndex;
		queueCreateInfo.queueCount       = 1;
		queueCreateInfo.pQueuePriorities = priorities;

		VkPhysicalDeviceFeatures enabledFeatures = {};

		VkDeviceCreateInfo createInfo      = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext                   = nullptr;
		createInfo.flags                   = 0;
		createInfo.queueCreateInfoCount    = 1;
		createInfo.pQueueCreateInfos       = &queueCreateInfo;
		createInfo.enabledLayerCount       = 0;
		createInfo.ppEnabledLayerNames     = nullptr;
		createInfo.enabledExtensionCount   = 0;
		createInfo.ppEnabledExtensionNames = nullptr;
		createInfo.pEnabledFeatures        = &enabledFeatures;

		VkCall({}, vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device),
		       "Failed to create VkDevice");

		vkGetDeviceQueue(m_Device, 0, 0, &m_GraphicsQueue);
	}

	void VulkanDevice::destroy()
	{
		if (m_Device)
			vkDestroyDevice(m_Device, nullptr);
		m_PhysicalDevice = nullptr;
		m_Device         = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
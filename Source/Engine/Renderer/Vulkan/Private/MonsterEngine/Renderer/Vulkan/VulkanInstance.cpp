#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"

#include <MonsterEngine/Core/ModuleVersion.h>
#include <MonsterEngine/Logger/Logger.h>

#include <stdexcept>
#include <vector>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanInstance::VulkanInstance(const std::string& name, Version version)
	{
		VkApplicationInfo appInfo  = {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext              = nullptr;
		appInfo.pApplicationName   = name.c_str();
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, version.m_Major, version.m_Minor, version.m_Patch);
		appInfo.pEngineName        = "MonsterEngine";
		appInfo.engineVersion      = VK_MAKE_API_VERSION(0, MonsterEngine::Core::gVersion.m_Major, MonsterEngine::Core::gVersion.m_Minor, MonsterEngine::Core::gVersion.m_Patch);
		appInfo.apiVersion         = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo    = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext                   = nullptr;
		createInfo.flags                   = 0;
		createInfo.pApplicationInfo        = &appInfo;
		createInfo.enabledLayerCount       = 0;
		createInfo.ppEnabledLayerNames     = nullptr;
		createInfo.enabledExtensionCount   = 0;
		createInfo.ppEnabledExtensionNames = nullptr;

		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create vulkan instance!");
	}

	VulkanInstance::~VulkanInstance()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

	std::unique_ptr<RHI::IDevice> VulkanInstance::findDevice()
	{
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, physicalDevices.data());

		VkPhysicalDevice bestPhysicalDevice = physicalDevices.empty() ? nullptr : physicalDevices[0];

		if (!bestPhysicalDevice)
			throw std::runtime_error("Failed to find an appropriate vulkan physical device!");

		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(bestPhysicalDevice, &properties);
		Logger::Trace("Found vulkan physical device '{}'", properties.deviceName);

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

		VkDevice device;
		if (vkCreateDevice(bestPhysicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
			throw std::runtime_error("Failed to create vulkan device");
		return std::make_unique<VulkanDevice>(this, device);
	}
} // namespace MonsterEngine::Renderer::Vulkan
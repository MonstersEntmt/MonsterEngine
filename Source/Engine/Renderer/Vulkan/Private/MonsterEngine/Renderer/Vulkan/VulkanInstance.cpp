#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"

#include <MonsterEngine/Core/ModuleVersion.h>
#include <MonsterEngine/Logger/Logger.h>

#include <stdexcept>
#include <vector>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanInstance::VulkanInstance(const std::string& name, Version version)
	    : IInstance(name), m_Version(version)
	{
		create();
	}

	VulkanInstance::~VulkanInstance()
	{
		VulkanInstance::destroy();
	}

	std::unique_ptr<RHI::IDevice> VulkanInstance::findDevice()
	{
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, physicalDevices.data());

		VkPhysicalDevice bestPhysicalDevice = physicalDevices.empty() ? nullptr : physicalDevices[0];
		for (auto physicalDevice : physicalDevices)
		{
			VkPhysicalDeviceProperties properties = {};
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);
			Logger::Trace("Found vulkan physical device '{}'", properties.deviceName);
		}

		if (!bestPhysicalDevice)
			throw std::runtime_error("Failed to find an appropriate vulkan physical device!");

		VkPhysicalDeviceProperties bestProperties = {};
		vkGetPhysicalDeviceProperties(bestPhysicalDevice, &bestProperties);
		Logger::Trace("Using vulkan physical device '{}'", bestProperties.deviceName);

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
		return std::make_unique<VulkanDevice>(std::string { bestProperties.deviceName }, this, bestPhysicalDevice, device);
	}

	void VulkanInstance::create()
	{
		Version engineVersion = MonsterEngine::Core::gVersion;

		VkApplicationInfo appInfo  = {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext              = nullptr;
		appInfo.pApplicationName   = getName().c_str();
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, m_Version.m_Major, m_Version.m_Minor, m_Version.m_Patch);
		appInfo.pEngineName        = "MonsterEngine";
		appInfo.engineVersion      = VK_MAKE_API_VERSION(0, engineVersion.m_Major, engineVersion.m_Minor, engineVersion.m_Patch);
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

	void VulkanInstance::destroy()
	{
		if (m_Instance)
			vkDestroyInstance(m_Instance, nullptr);
		m_Instance = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
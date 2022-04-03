#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanHelper.h"

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
		auto logger = Logger("Vulkan");

		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		if (!deviceCount)
			logger.exception({}, "Found no physical devices!");

		std::vector<VkPhysicalDevice> physicalDevices { deviceCount };
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, physicalDevices.data());

		VkPhysicalDevice bestPhysicalDevice = physicalDevices[0];
		for (auto physicalDevice : physicalDevices)
		{
			VkPhysicalDeviceProperties properties = {};
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);
			logger.trace("Found physical device '{}'", properties.deviceName);
		}

		if (!bestPhysicalDevice)
			logger.exception({}, "Failed to find an appropriate physical device!");

		VkPhysicalDeviceProperties bestProperties = {};
		vkGetPhysicalDeviceProperties(bestPhysicalDevice, &bestProperties);
		logger.trace("Using physical device '{}'", bestProperties.deviceName);

		return std::make_unique<VulkanDevice>(std::string { bestProperties.deviceName }, this, bestPhysicalDevice);
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

		VkCall({}, vkCreateInstance(&createInfo, nullptr, &m_Instance),
		       "Failed to create VkInstance!");
	}

	void VulkanInstance::destroy()
	{
		if (m_Instance)
			vkDestroyInstance(m_Instance, nullptr);
		m_Instance = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
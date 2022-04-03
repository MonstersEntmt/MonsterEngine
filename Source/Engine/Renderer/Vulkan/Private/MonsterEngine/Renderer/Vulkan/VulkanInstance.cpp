#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanHelper.h"

#include <MonsterEngine/Core/ModuleVersion.h>
#include <MonsterEngine/Logger/Logger.h>
#include <MonsterEngine/WindowManager/WindowManager.h>

#include <GLFW/glfw3.h>

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
		WindowManager::WindowManager::Get();

		std::vector<const char*> enabledLayerNames;
		std::vector<const char*> enabledExtensionNames;

		std::uint32_t glfwExtensionCount = 0;
		const char**  glfwExtensions     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		enabledExtensionNames.resize(glfwExtensionCount);
		for (std::size_t i = 0; i < glfwExtensionCount; ++i)
			enabledExtensionNames[i] = glfwExtensions[i];

		if (VulkanDebug::IsEnabled())
		{
			enabledLayerNames.push_back("VK_LAYER_KHRONOS_validation");
			enabledExtensionNames.push_back("VK_EXT_debug_utils");
		}

		Version engineVersion = MonsterEngine::Core::gVersion;

		VkApplicationInfo appInfo {};
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext              = nullptr;
		appInfo.pApplicationName   = getName().c_str();
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, m_Version.m_Major, m_Version.m_Minor, m_Version.m_Patch);
		appInfo.pEngineName        = "MonsterEngine";
		appInfo.engineVersion      = VK_MAKE_API_VERSION(0, engineVersion.m_Major, engineVersion.m_Minor, engineVersion.m_Patch);
		appInfo.apiVersion         = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext                   = nullptr;
		createInfo.flags                   = 0;
		createInfo.pApplicationInfo        = &appInfo;
		createInfo.enabledLayerCount       = static_cast<std::uint32_t>(enabledLayerNames.size());
		createInfo.ppEnabledLayerNames     = enabledLayerNames.data();
		createInfo.enabledExtensionCount   = static_cast<std::uint32_t>(enabledExtensionNames.size());
		createInfo.ppEnabledExtensionNames = enabledExtensionNames.data();

		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo {};
		if (VulkanDebug::IsEnabled())
		{
			VulkanDebug::PopulateCreateInfo(messengerCreateInfo);
			createInfo.pNext = &messengerCreateInfo;
		}

		VkCall({}, vkCreateInstance(&createInfo, nullptr, &m_Instance),
		       "Failed to create VkInstance!");

		m_Debug = std::make_unique<VulkanDebug>(this);
	}

	void VulkanInstance::destroy()
	{
		if (m_Instance)
			vkDestroyInstance(m_Instance, nullptr);
		m_Instance = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
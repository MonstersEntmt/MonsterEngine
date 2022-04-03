#include "MonsterEngine/Renderer/Vulkan/VulkanDebug.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanHelper.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"

#include <MonsterEngine/Logger/Logger.h>

namespace MonsterEngine::Renderer::Vulkan
{
	bool VulkanDebug::s_Enabled = s_IsConfigDebug;

	void VulkanDebug::Disable()
	{
		s_Enabled = false;
	}

	bool VulkanDebug::IsEnabled()
	{
		return s_Enabled;
	}

	void VulkanDebug::PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.pNext           = nullptr;
		createInfo.flags           = 0;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = &VulkanDebug::DebugCallback;
		createInfo.pUserData       = nullptr;
	}

	VkBool32 VulkanDebug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		auto logger = Logger("Vulkan");
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			logger.trace(pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			logger.info(pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			logger.warn(pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			logger.error(pCallbackData->pMessage);
			break;
		default:
			logger.info(pCallbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}

	VulkanDebug::VulkanDebug(VulkanInstance* instance)
	    : RHI::RHINode("Debug"), m_Instance(instance)
	{
		instance->addChild(this);
		create();
	}

	VulkanDebug::~VulkanDebug()
	{
		VulkanDebug::destroy();
	}

	RHI::IInstance* VulkanDebug::getInstance() const
	{
		return static_cast<RHI::IInstance*>(m_Instance);
	}

	void VulkanDebug::create()
	{
		auto instance = m_Instance->getHandle();

		VkDebugUtilsMessengerCreateInfoEXT createInfo {};
		PopulateCreateInfo(createInfo);
		PFN_vkCreateDebugUtilsMessengerEXT func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
		if (!func)
		{
			Logger("Vulkan").exception({}, "vkCreateDebugUtilsMessengerEXT not found!");
			return;
		}
		VkCallNoThrow({}, func(m_Instance->getHandle(), &createInfo, nullptr, &m_Messenger),
		              "Failed to create Debug Utils Messenger");
	}

	void VulkanDebug::destroy()
	{
		auto instance = m_Instance->getHandle();

		PFN_vkDestroyDebugUtilsMessengerEXT func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (!func)
		{
			Logger("Vulkan").exception({}, "vkDestroyDebugUtilsMessengerEXT not found!");
			return;
		}
		func(m_Instance->getHandle(), m_Messenger, nullptr);
	}
} // namespace MonsterEngine::Renderer::Vulkan
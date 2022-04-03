#pragma once

#include <MonsterEngine/Renderer/RHI/IInstance.h>
#include <MonsterEngine/Renderer/RHI/RHINode.h>

#include <vulkan/vulkan.h>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanInstance;

	class MonsterEngine_Renderer_Vulkan_API VulkanDebug : public RHI::RHINode
	{
	public:
		static void Disable();
		static bool IsEnabled();
		static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	private:
		static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	private:
		static bool s_Enabled;

	public:
		VulkanDebug(VulkanInstance* instance);
		~VulkanDebug();

		RHI::IInstance* getInstance() const;
		auto            getVulkanInstance() const { return m_Instance; }
		auto            getHandle() const { return m_Messenger; }

	private:
		void         create();
		virtual void destroy() override;

	private:
		VulkanInstance* m_Instance;

		VkDebugUtilsMessengerEXT m_Messenger;
	};
} // namespace MonsterEngine::Renderer::Vulkan
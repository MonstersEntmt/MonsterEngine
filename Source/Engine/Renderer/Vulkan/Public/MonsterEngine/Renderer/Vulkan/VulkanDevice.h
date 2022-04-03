#pragma once

#include <MonsterEngine/Renderer/RHI/IDevice.h>

#include <vulkan/vulkan.h>

#include <memory>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanInstance;

	class MonsterEngine_Renderer_Vulkan_API VulkanDevice : public RHI::IDevice
	{
	public:
		VulkanDevice(const std::string& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice);
		VulkanDevice(std::string&& name, VulkanInstance* instance, VkPhysicalDevice physicalDevice);
		~VulkanDevice();

		virtual std::unique_ptr<RHI::ISwapchain> newSwapchain(WindowManager::Window& window) override;

		VulkanInstance* getVulkanInstance() const;
		auto            getPhysicalDevice() const { return m_PhysicalDevice; }
		auto            getHandle() const { return m_Device; }

		auto getGraphicsQueue() const { return m_GraphicsQueue; }

	private:
		void         create();
		virtual void destroy() override;

	private:
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice         m_Device;

		VkQueue m_GraphicsQueue;
	};
} // namespace MonsterEngine::Renderer::Vulkan
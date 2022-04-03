#pragma once

#include <MonsterEngine/Renderer/RHI/ISwapchain.h>
#include <MonsterEngine/WindowManager/Window.h>

#include <vulkan/vulkan.h>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanDevice;

	class MonsterEngine_Renderer_Vulkan_API VulkanSwapchain : public RHI::ISwapchain
	{
	public:
		VulkanSwapchain(const std::string& name, VulkanDevice* device, WindowManager::Window& window);
		VulkanSwapchain(std::string&& name, VulkanDevice* device, WindowManager::Window& window);
		virtual ~VulkanSwapchain();

		virtual bool begin() override;
		virtual void end() override;

		VulkanDevice* getVulkanDevice() const;
		auto          getWindowId() const { return m_WindowId; }
		auto          getHandle() const { return m_Surface; }

	private:
		void         create();
		void         recreate();
		virtual void destroy() override;

	private:
		std::uint32_t m_WindowId;

		VkSurfaceKHR m_Surface;

		VkSwapchainKHR           m_Swapchain;
		std::vector<VkImage>     m_ColorImages;
		std::vector<VkImage>     m_DepthStencilImages;
		std::vector<VkImageView> m_ColorViews;
		std::vector<VkImageView> m_DepthViews;
		std::vector<VkImageView> m_StencilViews;

		VkSurfaceFormatKHR m_Format      = { VK_FORMAT_R8G8B8_SRGB, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		VkPresentModeKHR   m_PresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		std::uint32_t      m_Width = 0, m_Height = 0;
	};
} // namespace MonsterEngine::Renderer::Vulkan
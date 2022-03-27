#pragma once

#include <MonsterEngine/Renderer/RHI/ISurface.h>
#include <MonsterEngine/WindowManager/Window.h>

#include <vulkan/vulkan.h>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanDevice;

	class MonsterEngine_Renderer_Vulkan_API VulkanSurface : public RHI::ISurface
	{
	public:
		VulkanSurface(const std::string& name, VulkanDevice* device, WindowManager::Window& window);
		VulkanSurface(std::string&& name, VulkanDevice* device, WindowManager::Window& window);
		virtual ~VulkanSurface();

		auto getDevice() const { return m_Device; }
		auto getWindowId() const { return m_WindowId; }
		auto getHandle() const { return m_Surface; }

	private:
		void         create();
		virtual void destroy() override;

	private:
		VulkanDevice* m_Device;
		std::uint32_t m_WindowId;
		VkSurfaceKHR  m_Surface;
	};
} // namespace MonsterEngine::Renderer::Vulkan
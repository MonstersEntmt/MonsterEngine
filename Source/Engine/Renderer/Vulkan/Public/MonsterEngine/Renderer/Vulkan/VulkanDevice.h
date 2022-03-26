#pragma once

#include <MonsterEngine/Renderer/RHI/IDevice.h>
#include <MonsterEngine/Renderer/RHI/ISurface.h>

#include <vulkan/vulkan.h>

#include <memory>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanInstance;

	class MonsterEngine_Renderer_Vulkan_API VulkanDevice : public RHI::IDevice
	{
	public:
		VulkanDevice(VulkanInstance* instance, VkDevice device);
		~VulkanDevice();

		virtual std::unique_ptr<RHI::ISurface> newSurface(WindowManager::Window& window) override;

		auto getInstance() const { return m_Instance; }
		auto getHandle() const { return m_Device; }

	private:
		VulkanInstance* m_Instance;
		VkDevice        m_Device;
	};
} // namespace MonsterEngine::Renderer::Vulkan
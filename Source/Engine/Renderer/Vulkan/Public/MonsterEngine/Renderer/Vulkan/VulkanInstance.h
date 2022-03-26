#pragma once

#include <MonsterEngine/Renderer/RHI/IInstance.h>
#include <Version.h>

#include <vulkan/vulkan.h>

#include <string>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanInstance : public RHI::IInstance
	{
	public:
		VulkanInstance(const std::string& name, Version version);
		virtual ~VulkanInstance();

		virtual std::unique_ptr<RHI::IDevice> findDevice() override;

		auto getHandle() const { return m_Instance; }

	private:
		VkInstance m_Instance = nullptr;
	};
} // namespace MonsterEngine::Renderer::Vulkan
#pragma once

#include "VulkanDebug.h"

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
		auto getDebug() const { return m_Debug.get(); }

	private:
		void         create();
		virtual void destroy() override;

	private:
		Version m_Version;

		VkInstance m_Instance = nullptr;

		std::unique_ptr<VulkanDebug> m_Debug;
	};
} // namespace MonsterEngine::Renderer::Vulkan
#pragma once

#include <MonsterEngine/Renderer/RHI/IRHI.h>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API VulkanRHI : public RHI::IRHI
	{
	public:
		VulkanRHI();

		virtual bool isCompatible() override;

		virtual void setGLFWOptions(WindowManager::Window& window) override;

		virtual std::unique_ptr<RHI::IInstance> newInstance() override;
	};
} // namespace MonsterEngine::Renderer::Vulkan
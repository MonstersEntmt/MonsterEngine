#pragma once

#include <Module.h>
#include <MonsterEngine/Renderer/RHI/IRHI.h>

namespace MonsterEngine::Renderer::Vulkan
{
	class MonsterEngine_Renderer_Vulkan_API Module : public IMonsterModule
	{
	public:
		Module();

		virtual void startupModule() override;
		virtual void shutdownModule() override;

	private:
		RHI::IRHI* m_RHI;
	};
} // namespace MonsterEngine::Renderer::Vulkan
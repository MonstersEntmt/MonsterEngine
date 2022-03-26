#pragma once

#include <Module.h>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace MonsterEngine::Renderer::RHI
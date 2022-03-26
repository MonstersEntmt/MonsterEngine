#pragma once

#include <Module.h>

namespace MonsterEngine::Renderer::DirectX
{
	class MonsterEngine_Renderer_DirectX_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace MonsterEngine::Renderer::DirectX
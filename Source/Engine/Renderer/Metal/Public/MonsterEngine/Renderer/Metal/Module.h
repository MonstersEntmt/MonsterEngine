#pragma once

#include <Module.h>
#include <MonsterEngine/Renderer/RHI/IRHI.h>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API Module : public IMonsterModule
	{
	public:
		Module();

		virtual void startupModule() override;
		virtual void shutdownModule() override;

	private:
		RHI::IRHI* m_RHI;
	};
} // namespace MonsterEngine::Renderer::Metal
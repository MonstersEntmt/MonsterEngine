#include "MonsterEngine/Renderer/Metal/Module.h"
#include "MonsterEngine/Renderer/Metal/MetalRHI.h"
#include "MonsterEngine/Renderer/Metal/ModuleVersion.h"

#include <Module.h>
#include <MonsterEngine/Renderer/RHI/Registry.h>

namespace MonsterEngine::Renderer::Metal
{
	Module::Module()
	    : IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
		m_RHI = RHI::Registry::Get().registerRHI(std::make_unique<MetalRHI>());
	}

	void Module::shutdownModule()
	{
		RHI::Registry::Get().unregisterRHI(m_RHI);
	}
} // namespace MonsterEngine::Renderer::Metal

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::Metal::Module)
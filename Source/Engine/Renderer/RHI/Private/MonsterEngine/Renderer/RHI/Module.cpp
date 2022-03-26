#include "MonsterEngine/Renderer/RHI/Module.h"
#include "MonsterEngine/Renderer/RHI/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::Renderer::RHI
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::Renderer::RHI

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::RHI::Module)
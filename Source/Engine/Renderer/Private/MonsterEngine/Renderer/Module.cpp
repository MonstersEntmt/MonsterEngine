#include "MonsterEngine/Renderer/Module.h"
#include "MonsterEngine/Renderer/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::Renderer
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::Renderer

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::Module)
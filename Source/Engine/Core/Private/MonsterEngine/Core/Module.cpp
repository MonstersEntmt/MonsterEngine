#include "MonsterEngine/Core/Module.h"
#include "MonsterEngine/Core/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::Core
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::Core

ME_IMPLEMENT_MODULE(MonsterEngine::Core::Module)
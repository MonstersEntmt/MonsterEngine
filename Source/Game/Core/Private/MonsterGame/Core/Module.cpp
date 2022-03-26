#include "MonsterGame/Core/Module.h"
#include "MonsterGame/Core/ModuleVersion.h"

#include <Module.h>

namespace MonsterGame::Core
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterGame::Core

ME_IMPLEMENT_MODULE(MonsterGame::Core::Module)
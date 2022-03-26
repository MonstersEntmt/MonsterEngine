#include "MonsterEngine/WindowManager/Module.h"
#include "MonsterEngine/WindowManager/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::WindowManager
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::WindowManager

ME_IMPLEMENT_MODULE(MonsterEngine::WindowManager::Module)
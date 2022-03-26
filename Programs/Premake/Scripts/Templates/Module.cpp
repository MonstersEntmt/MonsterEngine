#include "%-Path-%Module.h"
#include "%-Path-%ModuleVersion.h"

#include <Module.h>

namespace %-Namespace-%
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace %-Namespace-%

ME_IMPLEMENT_MODULE(%-Namespace-%::Module)
#include "MonsterEngine/Renderer/Vulkan/Module.h"
#include "MonsterEngine/Renderer/Vulkan/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::Renderer::Vulkan
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::Renderer::Vulkan

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::Vulkan::Module)
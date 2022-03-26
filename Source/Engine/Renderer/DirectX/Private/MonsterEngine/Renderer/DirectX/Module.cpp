#include "MonsterEngine/Renderer/DirectX/Module.h"
#include "MonsterEngine/Renderer/DirectX/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::Renderer::DirectX
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::Renderer::DirectX

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::DirectX::Module)
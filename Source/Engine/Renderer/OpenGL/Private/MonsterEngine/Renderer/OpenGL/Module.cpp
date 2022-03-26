#include "MonsterEngine/Renderer/OpenGL/Module.h"
#include "MonsterEngine/Renderer/OpenGL/ModuleVersion.h"

#include <Module.h>

namespace MonsterEngine::Renderer::OpenGL
{
	Module::Module()
		: IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
	}
} // namespace MonsterEngine::Renderer::OpenGL

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::OpenGL::Module)
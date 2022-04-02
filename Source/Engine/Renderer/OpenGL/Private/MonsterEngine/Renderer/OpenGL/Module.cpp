#include "MonsterEngine/Renderer/OpenGL/Module.h"
#include "MonsterEngine/Renderer/OpenGL/ModuleVersion.h"
#include "MonsterEngine/Renderer/OpenGL/OpenGLRHI.h"

#include <Module.h>
#include <MonsterEngine/Renderer/RHI/Registry.h>

namespace MonsterEngine::Renderer::OpenGL
{
	Module::Module()
	    : IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
		m_RHI = RHI::Registry::Get().registerRHI(std::make_unique<OpenGLRHI>());
	}

	void Module::shutdownModule()
	{
		RHI::Registry::Get().unregisterRHI(m_RHI);
	}
} // namespace MonsterEngine::Renderer::OpenGL

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::OpenGL::Module)
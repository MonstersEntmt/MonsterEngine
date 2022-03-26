#include "MonsterEngine/Renderer/Vulkan/Module.h"
#include "MonsterEngine/Renderer/Vulkan/ModuleVersion.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanRHI.h"

#include <Module.h>
#include <MonsterEngine/Renderer/RHI/Registry.h>

namespace MonsterEngine::Renderer::Vulkan
{
	Module::Module()
	    : IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
		m_RHI = RHI::Registry::Get().registerRHI(std::make_unique<VulkanRHI>());
	}

	void Module::shutdownModule()
	{
		RHI::Registry::Get().unregisterRHI(m_RHI);
	}
} // namespace MonsterEngine::Renderer::Vulkan

ME_IMPLEMENT_MODULE(MonsterEngine::Renderer::Vulkan::Module)
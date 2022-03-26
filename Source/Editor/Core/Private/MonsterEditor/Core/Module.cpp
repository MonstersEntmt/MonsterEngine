#include "MonsterEditor/Core/Module.h"
#include "MonsterEditor/Core/ModuleVersion.h"

#include <Module.h>
#include <MonsterEngine/Renderer/RHI/Registry.h>
#include <MonsterEngine/WindowManager/WindowManager.h>

namespace MonsterEditor::Core
{
	Module::Module()
	    : IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
	}

	void Module::shutdownModule()
	{
		using namespace MonsterEngine;

		Renderer::RHI::Registry::Destroy();
		WindowManager::WindowManager::Destroy();
	}

	void Module::run()
	{
		using namespace MonsterEngine;

		auto& rhiRegistry = Renderer::RHI::Registry::Get();
		m_RHI             = rhiRegistry.getRHI("Metal");
		if (!m_RHI)
			throw std::runtime_error("RHI 'Metal' could not be found!");
		m_Instance = m_RHI->newInstance();

		auto& windowManager = WindowManager::WindowManager::Get();
		m_Window            = windowManager.createWindow(1280, 720, "MonsterEditor");
		while (windowManager.update())
		{
		}
	}
} // namespace MonsterEditor::Core

ME_IMPLEMENT_MODULE(MonsterEditor::Core::Module)
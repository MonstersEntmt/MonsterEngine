#include "MonsterEditor/Core/Module.h"
#include "MonsterEditor/Core/ModuleVersion.h"

#include <Module.h>
#include <MonsterEngine/WindowManager/WindowManager.h>

namespace MonsterEditor::Core
{
	Module::Module()
	    : IMonsterModule(GetVersion()) {}

	void Module::startupModule()
	{
		using namespace MonsterEngine;

		m_Window = WindowManager::WindowManager::Get().createWindow(1280, 720, "MonsterEditor");
	}

	void Module::shutdownModule()
	{
		using namespace MonsterEngine;

		WindowManager::WindowManager::Destroy();
	}

	void Module::run()
	{
		using namespace MonsterEngine;

		auto& windowManager = WindowManager::WindowManager::Get();
		while (windowManager.update())
		{
		}
	}
} // namespace MonsterEditor::Core

ME_IMPLEMENT_MODULE(MonsterEditor::Core::Module)
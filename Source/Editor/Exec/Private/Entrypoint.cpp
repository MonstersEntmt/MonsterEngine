#include <MonsterEngine/Logger/Logger.h>
#include <MonsterEngine/ModuleManager/ModuleManager.h>

#include <cstdlib>

int main(int argc, char** argv)
{
	using namespace MonsterEngine;
	auto& moduleManager = ModuleManager::ModuleManager::Get();
	moduleManager.addSearchDir(std::filesystem::path { argv[0] }.parent_path());

	auto modules = moduleManager.getAvailableModules();
	for (auto& module : modules)
		Logger::Trace("Found module '{}' version {}.{}.{}", module.first, module.second.m_Version.m_Major, module.second.m_Version.m_Minor, module.second.m_Version.m_Patch);

	for (auto& module : modules)
		moduleManager.loadModule(module.first);

	auto editorCore = moduleManager.getModule("MonsterEditor.Core");
	editorCore->run();
	ModuleManager::ModuleManager::Destroy();
}

#if BUILD_IS_SYSTEM_WINDOWS

#undef APIENTRY
#include <Windows.h>

int WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd)
{
	return main(__argc, __argv);
}

#endif
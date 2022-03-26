#include "MonsterEngine/ModuleManager/ModuleManager.h"
#include "MonsterEngine/Logger/Logger.h"
#include "MonsterEngine/ModuleManager/ModuleVersion.h"
#include "MonsterEngine/Serializer/Ini.h"

#include <regex>
#include <utility>

#if BUILD_IS_SYSTEM_WINDOWS
#include <Windows.h>
#elif BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
#include <dlfcn.h>
#endif

namespace MonsterEngine::ModuleManager
{
	ModuleLoadException::ModuleLoadException(const std::string& message)
	    : std::runtime_error(message) {}

	ModuleLoadException::ModuleLoadException(std::string&& message)
	    : std::runtime_error(std::move(message)) {}

	std::int32_t ModuleManager::ModuleInfo::s_CurrentLoadOrder = 0;

	ModuleManager::ModuleInfo::ModuleInfo()
	    : m_LoadOrder(s_CurrentLoadOrder++) {}

	ModuleManager& ModuleManager::Get()
	{
		static ModuleManager* s_Instance = new ModuleManager();
		return *s_Instance;
	}

	void ModuleManager::Destroy()
	{
		delete &Get();
	}

	IMonsterModule* ModuleManager::loadModule(std::string_view name)
	{
		if (name == "MonsterEngine.ModuleManager")
			return nullptr;
		for (auto& module : m_Modules)
			if (module.first == name)
				return module.second.m_Module;

		std::filesystem::path modulePath;
		if (!isModuleUpToDate(name, modulePath))
			return nullptr;

		auto configFilepath = modulePath;
		configFilepath.replace_extension(".ini");

		Serializer::Ini ini;
		if (!Serializer::readFileNoexcept(configFilepath, ini))
			return nullptr;

		auto dependenciesSection = ini.getSection("Dependencies");
		if (!dependenciesSection)
			return nullptr;

		std::unordered_map<std::string, Version> dependencies;

		std::vector<std::uint32_t> versionArr;
		auto&                      dependenciesIni = dependenciesSection->getIni();
		for (auto& entry : dependenciesIni)
		{
			if (entry->getName() == "MonsterEngine.ModuleManager")
				continue;

			std::filesystem::path entryPath;
			if (!isModuleUpToDate(entry->getName(), entryPath))
				continue;

			Version actualVersion = getModuleVersion(entryPath);

			if (!entry->tryGetValueOfType(versionArr) || versionArr.size() < 3)
				continue;

			Version requestedVersion = { versionArr[0], versionArr[1], versionArr[2] };
			if (!actualVersion.isCompatibleWith(requestedVersion))
				return nullptr;

			dependencies.insert({ entry->getName(), actualVersion });
		}

		for (auto& dep : dependencies)
		{
			auto module = loadModule(dep.first);
			if (!module || !module->getVersion().isCompatibleWith(dep.second))
				throw ModuleLoadException("Requested module: \"" + dep.first + "\" was loaded incorrectly!");
		}

		auto module = loadModuleAtPath(modulePath);
		if (!module)
			throw ModuleLoadException("Requested module: \"" + std::string(name) + "\" was loaded incorrectly!");

		if (!ini["Version"]->tryGetValueOfType(versionArr) || versionArr.size() < 3)
		{
			unloadModuleFromInfo(*module);
			throw ModuleLoadException("Invalid module: \"" + std::string(name) + "\" version missing from ini file!");
		}

		module->m_Version = { versionArr[0], versionArr[1], versionArr[2] };
		auto version      = module->m_Module->getVersion();
		if (module->m_Version != version)
		{
			unloadModuleFromInfo(*module);
			throw ModuleLoadException("Invalid module: \"" + std::string(name) + "\" ini version not the same as the built version!");
		}

		Logger::Trace("Loaded module '{}' version {}.{}.{}", name, version.m_Major, version.m_Minor, version.m_Patch);
		module->m_Module->startupModule();
		return module->m_Module;
	}

	IMonsterModule* ModuleManager::getModule(const std::string& name)
	{
		auto itr = m_Modules.find(name);
		return itr != m_Modules.end() ? itr->second.m_Module : nullptr;
	}

	const IMonsterModule* ModuleManager::getModule(const std::string& name) const
	{
		auto itr = m_Modules.find(name);
		return itr != m_Modules.end() ? itr->second.m_Module : nullptr;
	}

	std::unordered_map<std::string, ModuleManager::ModuleInfo>& ModuleManager::getAvailableModules()
	{
		m_AvailableModules.clear();

		for (auto& searchDir : m_SearchDirs)
		{
			for (auto& pathItr : std::filesystem::recursive_directory_iterator(searchDir))
			{
				if (!pathItr.is_regular_file())
					continue;

				auto& path = pathItr.path();
#if BUILD_IS_SYSTEM_WINDOWS
				if (path.extension() != ".dll")
					continue;
#elif BUILD_IS_SYSTEM_LINUX
				if (path.extension() != ".so")
					continue;
#elif BUILD_IS_SYSTEM_MACOSX
				if (path.extension() != ".dylib")
					continue;
#endif

				auto filename = path.filename();
				filename.replace_extension("");
#if BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
				filename.replace_filename(filename.u8string().substr(3));
#endif

				auto configFilepath = path;
				configFilepath.replace_extension(".ini");
				if (std::filesystem::exists(configFilepath))
				{
					Serializer::Ini ini;
					if (!Serializer::readFileNoexcept(configFilepath, ini))
						continue;

					std::vector<std::uint32_t> versionArr;
					if (!ini["Version"]->tryGetValueOfType(versionArr) || versionArr.size() < 3)
						continue;

					auto& moduleInfo     = m_AvailableModules.insert({ filename.string(), {} }).first->second;
					moduleInfo.m_Version = { versionArr[0], versionArr[1], versionArr[2] };
				}
			}
		}

		return m_AvailableModules;
	}

	void ModuleManager::addSearchDir(const std::filesystem::path& searchDir)
	{
		m_SearchDirs.push_back(searchDir);
	}

	void ModuleManager::removeSearchDir(const std::filesystem::path& searchDir)
	{
		auto itr = std::find(m_SearchDirs.begin(), m_SearchDirs.end(), searchDir);
		if (itr != m_SearchDirs.end())
			m_SearchDirs.erase(itr);
	}

	ModuleManager::~ModuleManager()
	{
		std::vector<ModuleInfo*> sortedModules;
		for (auto& module : m_Modules)
			sortedModules.push_back(&module.second);

		std::sort(sortedModules.begin(), sortedModules.end(),
		          [](ModuleInfo* a, ModuleInfo* b) -> bool
		          {
			          return a->m_LoadOrder > b->m_LoadOrder;
		          });

		for (auto module : sortedModules)
			unloadModuleFromInfo(*module);
		m_Modules.clear();
	}

	bool ModuleManager::isModuleUpToDate(std::string_view name, std::filesystem::path& outPath) const
	{
		std::vector<std::filesystem::path> paths;
		findModulePaths(name, paths, false);
		for (auto& path : paths)
		{
			auto configFilepath = path;
			configFilepath.replace_extension(".ini");

			Serializer::Ini ini;
			if (!Serializer::readFileNoexcept(configFilepath, ini))
				continue;

			auto dependenciesSection = ini.getSection("Dependencies");
			if (!dependenciesSection)
				continue;

			auto& dependenciesIni  = dependenciesSection->getIni();
			auto  moduleManagerDep = dependenciesIni.getEntry("MonsterEngine.ModuleManager");
			if (!moduleManagerDep)
				continue;

			std::vector<std::uint32_t> versionArr;
			if (!moduleManagerDep->tryGetValueOfType(versionArr) || versionArr.size() < 3)
				continue;

			Version version = { versionArr[0], versionArr[1], versionArr[2] };
			if (gVersion.isCompatibleWith(version))
			{
				outPath = path;
				return true;
			}
		}
		return false;
	}

	Version ModuleManager::getModuleVersion(const std::filesystem::path& path) const
	{
		auto configFilepath = path;
		configFilepath.replace_extension(".ini");

		Serializer::Ini ini;
		if (!Serializer::readFileNoexcept(configFilepath, ini))
			return {};

		auto versionEntry = ini.getEntry("Version");
		if (!versionEntry)
			return {};

		std::vector<std::uint32_t> versionArr;
		if (!versionEntry->tryGetValueOfType(versionArr) || versionArr.size() < 3)
			return {};

		return { versionArr[0], versionArr[1], versionArr[2] };
	}

	void ModuleManager::findModulePaths(std::string_view nameRegex, std::vector<std::filesystem::path>& paths, bool useRegex) const
	{
		for (auto& searchDir : m_SearchDirs)
		{
			for (auto& pathItr : std::filesystem::recursive_directory_iterator(searchDir))
			{
				if (!pathItr.is_regular_file())
					continue;

				auto& path = pathItr.path();
#if BUILD_IS_SYSTEM_WINDOWS
				if (path.extension() != ".dll")
					continue;
#elif BUILD_IS_SYSTEM_LINUX
				if (path.extension() != ".so")
					continue;
#elif BUILD_IS_SYSTEM_MACOSX
				if (path.extension() != ".dylib")
					continue;
#endif

				auto filename = path.filename();
				filename.replace_extension("");
#if BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
				filename.replace_filename(filename.u8string().substr(3));
#endif
				if (useRegex && !std::regex_match(filename.string(), std::regex(nameRegex.begin(), nameRegex.end())))
					continue;
				else if (filename != nameRegex)
					continue;

				auto configFilepath = path;
				configFilepath.replace_extension(".ini");
				if (std::filesystem::exists(configFilepath))
					paths.push_back(path);
			}
		}
	}

	ModuleManager::ModuleInfo* ModuleManager::loadModuleAtPath(const std::filesystem::path& path)
	{
		std::string pathStr = path.string();

		void* library = nullptr;

		ModuleEntrypointFunc moduleEntrypointFunc = nullptr;
		ModuleExitpointFunc  moduleExitpointFunc  = nullptr;

#if BUILD_IS_SYSTEM_WINDOWS
		library = static_cast<void*>(LoadLibraryA(pathStr.c_str()));
		if (!library)
		{
			// TODO(MarcasRealAccount): Give error message if library could not be loaded.
			return nullptr;
		}

		moduleEntrypointFunc = reinterpret_cast<ModuleEntrypointFunc>(GetProcAddress(static_cast<HMODULE>(library), "gModuleEntrypoint"));
		moduleExitpointFunc  = reinterpret_cast<ModuleExitpointFunc>(GetProcAddress(static_cast<HMODULE>(library), "gModuleExitpoint"));
#elif BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
		library = dlopen(pathStr.c_str(), RTLD_NOW);
		if (!libray)
		{
			// TODO(MarcasRealAccount): Give error message if library could not be loaded.
			return nullptr;
		}

		moduleEntrypointFunc = reinterpret_cast<ModuleEntrypointFunc>(dlsym(library, "gModuleEntrypoint"));
		moduleExitpointFunc = reinterpret_cast<ModuleExitpointFunc>(dlsym(library, "gModuleExitpoint"));
#endif
		if (!moduleEntrypointFunc || !moduleExitpointFunc)
			return nullptr;

		IMonsterModule* module = moduleEntrypointFunc();

		auto filename = path.filename();
		filename.replace_extension("");
#if BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
		filename.replace_filename(filename.u8string().substr(3));
#endif
		auto& moduleInfo      = m_Modules.insert({ filename.string(), {} }).first->second;
		moduleInfo.m_Handle   = static_cast<void*>(library);
		moduleInfo.m_Module   = module;
		moduleInfo.m_Filepath = path;
		moduleInfo.m_ExitFunc = moduleExitpointFunc;
		return &moduleInfo;
	}

	void ModuleManager::unloadModuleFromInfo(ModuleInfo& info)
	{
		info.m_Module->shutdownModule();
		info.m_ExitFunc(info.m_Module);
#if BUILD_IS_SYSTEM_WINDOWS
		FreeLibrary(static_cast<HMODULE>(info.m_Handle));
#elif BUILD_IS_SYSTEM_MACOSX || BUILD_IS_SYSTEM_LINUX
		dlclose(info.m_Handle);
#endif
		info.m_Handle   = nullptr;
		info.m_Module   = nullptr;
		info.m_ExitFunc = nullptr;
	}
} // namespace MonsterEngine::ModuleManager
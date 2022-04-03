#pragma once

#include "Module.h"
#include "Version.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace MonsterEngine::ModuleManager
{
	class MonsterEngine_ModuleManager_API ModuleManager
	{
	private:
		struct MonsterEngine_ModuleManager_API ModuleInfo
		{
		public:
			static std::int32_t s_CurrentLoadOrder;

		public:
			ModuleInfo();

		public:
			std::string           m_Name;
			std::filesystem::path m_Filepath;
			Version               m_Version  = {};
			void*                 m_Handle   = nullptr;
			IMonsterModule*       m_Module   = nullptr;
			ModuleExitpointFunc   m_ExitFunc = nullptr;
			std::int32_t          m_LoadOrder;
		};

	public:
		static ModuleManager& Get();
		static void           Destroy();

	public:
		IMonsterModule* loadModule(std::string_view name);

		IMonsterModule*       getModule(const std::string& name);
		const IMonsterModule* getModule(const std::string& name) const;

		std::unordered_map<std::string, ModuleInfo>& getAvailableModules();

		void  addSearchDir(const std::filesystem::path& searchDir);
		void  removeSearchDir(const std::filesystem::path& searchDir);
		auto& getSearchDirs() const { return m_SearchDirs; }

	private:
		ModuleManager()                     = default;
		ModuleManager(const ModuleManager&) = delete;
		ModuleManager(ModuleManager&&)      = delete;
		ModuleManager& operator=(const ModuleManager&) = delete;
		ModuleManager& operator=(ModuleManager&&) = delete;
		~ModuleManager();

		bool    isModuleUpToDate(std::string_view name, std::filesystem::path& outPath) const;
		Version getModuleVersion(const std::filesystem::path& path) const;
		void    findModulePaths(std::string_view nameRegex, std::vector<std::filesystem::path>& paths, bool useRegex = true) const;

		ModuleInfo* loadModuleAtPath(const std::filesystem::path& path);
		void        unloadModuleFromInfo(ModuleInfo& info);

	private:
		std::unordered_map<std::string, ModuleInfo> m_AvailableModules;
		std::unordered_map<std::string, ModuleInfo> m_Modules;

		std::vector<std::filesystem::path> m_SearchDirs;
	};
} // namespace MonsterEngine::ModuleManager
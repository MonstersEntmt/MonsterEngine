#pragma once

#include "Version.h"

#include <string>

namespace MonsterEngine::ModuleManager
{
	class MonsterEngine_ModuleManager_API ModuleManager;
}

class MonsterEngine_ModuleManager_API IMonsterModule
{
public:
	friend MonsterEngine::ModuleManager::ModuleManager;

public:
	IMonsterModule(Version version);
	virtual ~IMonsterModule() = default;

	virtual void startupModule()  = 0;
	virtual void shutdownModule() = 0;

	virtual void run() {}

	void* getProcAddress(const std::string& name);

	auto getNativeHandle() const { return m_NativeHandle; }
	auto getVersion() const { return m_Version; }

private:
	void*   m_NativeHandle = nullptr;
	Version m_Version;
};

#define ME_IMPLEMENT_MODULE(mod)                                       \
	extern "C"                                                         \
	{                                                                  \
		ME_MODULE_EXPORT IMonsterModule* gModuleEntrypoint()           \
		{                                                              \
			return new mod();                                          \
		}                                                              \
		ME_MODULE_EXPORT void gModuleExitpoint(IMonsterModule* module) \
		{                                                              \
			delete module;                                             \
		}                                                              \
	}

using ModuleEntrypointFunc = IMonsterModule* (*) ();
using ModuleExitpointFunc  = void (*)(IMonsterModule*);
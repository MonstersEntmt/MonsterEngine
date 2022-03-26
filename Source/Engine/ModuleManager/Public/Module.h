#pragma once

#include "Version.h"

class MonsterEngine_ModuleManager_API IMonsterModule
{
public:
	IMonsterModule(Version version);
	virtual ~IMonsterModule() = default;

	virtual void startupModule()  = 0;
	virtual void shutdownModule() = 0;

	virtual void run() {}

	auto getVersion() const { return m_Version; }

private:
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
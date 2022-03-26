#pragma once

#include "Version.h"

namespace MonsterEngine::ModuleManager
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_ModuleManager_API Version GetVersion();
} // namespace MonsterEngine::ModuleManager
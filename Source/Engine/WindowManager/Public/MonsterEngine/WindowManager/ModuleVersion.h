#pragma once

#include <Version.h>

namespace MonsterEngine::WindowManager
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_WindowManager_API Version GetVersion();
} // namespace MonsterEngine::WindowManager
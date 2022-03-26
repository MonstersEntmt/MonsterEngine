#pragma once

#include <Version.h>

namespace MonsterEngine::Core
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Core_API Version GetVersion();
} // namespace MonsterEngine::Core
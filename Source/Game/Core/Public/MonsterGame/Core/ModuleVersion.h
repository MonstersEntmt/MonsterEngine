#pragma once

#include <Version.h>

namespace MonsterGame::Core
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterGame_Core_API Version GetVersion();
} // namespace MonsterGame::Core
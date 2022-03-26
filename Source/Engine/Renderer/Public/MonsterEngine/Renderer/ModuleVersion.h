#pragma once

#include <Version.h>

namespace MonsterEngine::Renderer
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Renderer_API Version GetVersion();
} // namespace MonsterEngine::Renderer
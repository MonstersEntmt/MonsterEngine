#pragma once

#include <Version.h>

namespace MonsterEngine::Renderer::Metal
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Renderer_Metal_API Version GetVersion();
} // namespace MonsterEngine::Renderer::Metal
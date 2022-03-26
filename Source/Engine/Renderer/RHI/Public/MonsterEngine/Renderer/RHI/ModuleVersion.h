#pragma once

#include <Version.h>

namespace MonsterEngine::Renderer::RHI
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Renderer_RHI_API Version GetVersion();
} // namespace MonsterEngine::Renderer::RHI
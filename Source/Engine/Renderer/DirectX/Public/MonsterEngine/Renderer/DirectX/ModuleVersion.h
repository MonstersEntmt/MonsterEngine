#pragma once

#include <Version.h>

namespace MonsterEngine::Renderer::DirectX
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Renderer_DirectX_API Version GetVersion();
} // namespace MonsterEngine::Renderer::DirectX
#pragma once

#include <Version.h>

namespace MonsterEngine::Renderer::Vulkan
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Renderer_Vulkan_API Version GetVersion();
} // namespace MonsterEngine::Renderer::Vulkan
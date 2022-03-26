#pragma once

#include <Version.h>

namespace MonsterEngine::Renderer::OpenGL
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEngine_Renderer_OpenGL_API Version GetVersion();
} // namespace MonsterEngine::Renderer::OpenGL
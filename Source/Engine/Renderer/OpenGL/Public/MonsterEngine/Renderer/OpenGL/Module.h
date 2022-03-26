#pragma once

#include <Module.h>

namespace MonsterEngine::Renderer::OpenGL
{
	class MonsterEngine_Renderer_OpenGL_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace MonsterEngine::Renderer::OpenGL
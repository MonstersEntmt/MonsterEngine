#pragma once

#include <Module.h>

namespace MonsterEngine::WindowManager
{
	class MonsterEngine_WindowManager_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace MonsterEngine::WindowManager
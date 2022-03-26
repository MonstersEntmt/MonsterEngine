#pragma once

#include <Module.h>

namespace MonsterEngine::Core
{
	class MonsterEngine_Core_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace MonsterEngine::Core
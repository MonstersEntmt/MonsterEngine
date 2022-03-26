#pragma once

#include <Module.h>

namespace MonsterGame::Core
{
	class MonsterGame_Core_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace MonsterGame::Core
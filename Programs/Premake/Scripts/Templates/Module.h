#pragma once

#include <Module.h>

namespace %-Namespace-%
{
	class %-DefinePath-%_API Module : public IMonsterModule
	{
	public:
		Module();
		
		virtual void startupModule() override;
		virtual void shutdownModule() override;
	};
} // namespace %-Namespace-%
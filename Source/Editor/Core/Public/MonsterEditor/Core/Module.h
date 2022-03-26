#pragma once

#include <Module.h>

#include <cstdint>

namespace MonsterEditor::Core
{
	class MonsterEditor_Core_API Module : public IMonsterModule
	{
	public:
		Module();

		virtual void startupModule() override;
		virtual void shutdownModule() override;

		virtual void run() override;

	private:
		std::uint32_t m_Window = 0;
	};
} // namespace MonsterEditor::Core
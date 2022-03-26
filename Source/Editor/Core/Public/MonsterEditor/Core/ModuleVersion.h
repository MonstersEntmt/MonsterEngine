#pragma once

#include <Version.h>

namespace MonsterEditor::Core
{
	static constexpr Version gVersion { 0, 1, 0 };

	MonsterEditor_Core_API Version GetVersion();
} // namespace MonsterEditor::Core
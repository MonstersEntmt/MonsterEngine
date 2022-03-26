#pragma once

#include %-VersionH-%

namespace %-Namespace-%
{
	static constexpr Version gVersion { %-Version-% };

	%-DefinePath-%_API Version GetVersion();
} // namespace %-Namespace-%
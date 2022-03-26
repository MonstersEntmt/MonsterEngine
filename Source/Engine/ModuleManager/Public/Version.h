#pragma once

#include <cstdint>

struct Version
{
public:
	bool isCompatibleWith(Version other) const;

	friend bool operator==(Version lhs, Version rhs) { return lhs.m_Major == rhs.m_Major && lhs.m_Minor == rhs.m_Minor && lhs.m_Patch == rhs.m_Patch; }
	friend bool operator!=(Version lhs, Version rhs) { return !(lhs == rhs); }

public:
	std::uint32_t m_Major, m_Minor, m_Patch;
};
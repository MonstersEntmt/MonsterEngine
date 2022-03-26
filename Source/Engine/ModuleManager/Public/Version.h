#pragma once

#include <cstdint>

struct Version
{
public:
	bool isCompatibleWith(Version other) const;

public:
	std::uint32_t m_Major, m_Minor, m_Patch;
};
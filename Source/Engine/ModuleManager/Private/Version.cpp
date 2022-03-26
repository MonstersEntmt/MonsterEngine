#include "Version.h"

bool Version::isCompatibleWith(Version other) const
{
	return m_Major == other.m_Major && m_Minor >= other.m_Major;
}
#include "MonsterEngine/Renderer/RHI/IRHI.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	IRHI::IRHI(const std::string& name)
	    : m_Name(name) {}

	IRHI::IRHI(std::string&& name)
	    : m_Name(std::move(name)) {}
} // namespace MonsterEngine::Renderer::RHI
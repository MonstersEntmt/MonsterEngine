#include "MonsterEngine/Renderer/RHI/IRHI.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	IRHI::IRHI(const std::string& name)
	    : m_Name(name) {}

	IRHI::IRHI(std::string&& name)
	    : m_Name(std::move(name)) {}

	void IRHI::init()
	{
		m_Instance = newInstance();
		m_Device   = m_Instance->findDevice();
	}
} // namespace MonsterEngine::Renderer::RHI
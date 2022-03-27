#include "MonsterEngine/Renderer/RHI/IRHI.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	IRHI::IRHI(const std::string& name)
	    : RHINode(name) {}

	IRHI::IRHI(std::string&& name)
	    : RHINode(std::move(name)) {}

	void IRHI::init()
	{
		m_Instance = newInstance();
		m_Device   = m_Instance->findDevice();
	}

	void IRHI::destroy()
	{
		m_Device   = nullptr;
		m_Instance = nullptr;
	}
} // namespace MonsterEngine::Renderer::RHI
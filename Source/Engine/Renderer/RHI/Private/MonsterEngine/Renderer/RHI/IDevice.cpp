#include "MonsterEngine/Renderer/RHI/IDevice.h"
#include "MonsterEngine/Renderer/RHI/IInstance.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	IDevice::IDevice(const std::string& name, IInstance* instance)
	    : RHINode(name), m_Instance(instance)
	{
		m_Instance->addChild(this);
	}

	IDevice::IDevice(std::string&& name, IInstance* instance)
	    : RHINode(std::move(name)), m_Instance(instance)
	{
		m_Instance->addChild(this);
	}
} // namespace MonsterEngine::Renderer::RHI
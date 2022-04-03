#include "MonsterEngine/Renderer/RHI/ISwapchain.h"
#include "MonsterEngine/Renderer/RHI/IDevice.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	ISwapchain::ISwapchain(const std::string& name, IDevice* device)
	    : RHINode(name), m_Device(device)
	{
		m_Device->addChild(this);
	}

	ISwapchain::ISwapchain(std::string&& name, IDevice* device)
	    : RHINode(std::move(name)), m_Device(device)
	{
		m_Device->addChild(this);
	}
} // namespace MonsterEngine::Renderer::RHI
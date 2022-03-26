#include "MonsterEngine/Renderer/Metal/MetalDevice.h"

namespace MonsterEngine::Renderer::Metal
{
	MetalDevice::MetalDevice(MTL::Device* device)
	    : m_Device(device) {}

	MetalDevice::~MetalDevice()
	{
		m_Device->release();
	}
} // namespace MonsterEngine::Renderer::Metal
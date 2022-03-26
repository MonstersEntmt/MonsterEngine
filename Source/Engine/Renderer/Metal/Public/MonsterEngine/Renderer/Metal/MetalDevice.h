#pragma once

#include "Platforms/Metal/Metal/MTLDevice.hpp"

#include <MonsterEngine/Renderer/RHI/IDevice.h>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API MetalDevice : public RHI::IDevice
	{
	public:
		MetalDevice(MTL::Device* device);
		~MetalDevice();

	private:
		MTL::Device* m_Device;
	};
} // namespace MonsterEngine::Renderer::Metal
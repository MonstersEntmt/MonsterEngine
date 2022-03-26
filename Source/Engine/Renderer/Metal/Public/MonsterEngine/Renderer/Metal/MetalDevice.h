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

		virtual std::unique_ptr<RHI::ISurface> newSurface(WindowManager::Window& window) override;

	private:
		MTL::Device* m_Device;
	};
} // namespace MonsterEngine::Renderer::Metal
#include "MonsterEngine/Renderer/Metal/MetalInstance.h"
#include "MonsterEngine/Renderer/Metal/MetalDevice.h"
#include "Platforms/Metal/Foundation/Foundation.hpp"
#include "Platforms/Metal/Metal/Metal.hpp"
#include "Platforms/Metal/QuartzCore/QuartzCore.hpp"

#include <MonsterEngine/Logger/Logger.h>

namespace MonsterEngine::Renderer::Metal
{
	MetalInstance::MetalInstance()
	{
	}

	MetalInstance::~MetalInstance()
	{
	}

	std::unique_ptr<RHI::IDevice> MetalInstance::findDevice()
	{
		MTL::Device* device = MTL::CreateSystemDefaultDevice();
		Logger("Metal").Trace("Using device '{}'", device->name()->utf8String());
		return std::make_unique<MetalDevice>(device);
	}
} // namespace MonsterEngine::Renderer::Metal
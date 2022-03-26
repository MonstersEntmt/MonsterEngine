#include "MonsterEngine/Renderer/Metal/MetalInstance.h"
#include "Platforms/Metal/Foundation/Foundation.hpp"
#include "Platforms/Metal/Metal/Metal.hpp"
#include "Platforms/Metal/QuartzCore/QuartzCore.hpp"

namespace MonsterEngine::Renderer::Metal
{
	MetalInstance::MetalInstance()
	{
		MTL::CreateSystemDefaultDevice();
	}

	MetalInstance::~MetalInstance()
	{
	}
} // namespace MonsterEngine::Renderer::Metal
#include "MonsterEngine/Renderer/Metal/MetalRHI.h"
#include "MonsterEngine/Renderer/Metal/MetalInstance.h"

namespace MonsterEngine::Renderer::Metal
{
	MetalRHI::MetalRHI()
	    : RHI::IRHI("Metal") {}

	bool MetalRHI::isCompatible()
	{
	}

	std::unique_ptr<RHI::IInstance> MetalRHI::newInstance()
	{
		return std::make_unique<MetalInstance>();
	}
} // namespace MonsterEngine::Renderer::Metal
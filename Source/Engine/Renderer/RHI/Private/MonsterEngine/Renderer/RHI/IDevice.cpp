#include "MonsterEngine/Renderer/RHI/IDevice.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	IDevice::IDevice(const std::string& name)
	    : RHINode(name) {}

	IDevice::IDevice(std::string&& name)
	    : RHINode(std::move(name)) {}
} // namespace MonsterEngine::Renderer::RHI
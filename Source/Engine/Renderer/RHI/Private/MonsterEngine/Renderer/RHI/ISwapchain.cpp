#include "MonsterEngine/Renderer/RHI/ISwapchain.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	ISwapchain::ISwapchain(const std::string& name)
	    : RHINode(name) {}

	ISwapchain::ISwapchain(std::string&& name)
	    : RHINode(std::move(name)) {}
} // namespace MonsterEngine::Renderer::RHI
#include "MonsterEngine/Renderer/Metal/MetalSwapchain.h"

namespace MonsterEngine::Renderer::Metal
{
	MetalSwapchain::MetalSwapchain(WindowManager::Window& window, CAMetalLayer* metalLayer)
	    : m_WindowID(window.getId()), m_MetalLayer(metalLayer)
	{
	}

	MetalSwapchain::~MetalSwapchain()
	{
		[m_MetalLayer release];
	}
} // namespace MonsterEngine::Renderer::Metal
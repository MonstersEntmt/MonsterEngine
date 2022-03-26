#include "MonsterEngine/Renderer/Metal/MetalSurface.h"

namespace MonsterEngine::Renderer::Metal
{
	MetalSurface::MetalSurface(WindowManager::Window& window, CAMetalLayer* metalLayer)
	    : m_WindowID(window.getId()), m_MetalLayer(metalLayer)
	{
	}

	MetalSurface::~MetalSurface()
	{
		[m_MetalLayer release];
	}
} // namespace MonsterEngine::Renderer::Metal
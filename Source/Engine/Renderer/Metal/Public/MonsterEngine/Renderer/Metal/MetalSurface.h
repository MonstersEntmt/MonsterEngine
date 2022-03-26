#pragma once

#include "Platforms/Metal/QuartzCore/CAMetalDrawable.hpp"

#include <MonsterEngine/Renderer/RHI/ISurface.h>
#include <MonsterEngine/WindowManager/Window.h>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API MetalSurface : public RHI::ISurface
	{
	public:
		MetalSurface(WindowManager::Window& window, CA::MetalLayer* metalLayer);
		virtual ~MetalSurface();

		auto getWindowID() const { return m_WindowID; }

	private:
		std::uint32_t   m_WindowID;
		CA::MetalLayer* m_MetalLayer;
	};
} // namespace MonsterEngine::Renderer::Metal
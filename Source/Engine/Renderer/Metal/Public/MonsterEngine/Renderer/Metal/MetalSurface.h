#pragma once

#include "Platforms/Metal/Foundation/Foundation.hpp"
#include "Platforms/Metal/Metal/Metal.hpp"
#include "Platforms/Metal/QuartzCore/QuartzCore.hpp"

#include <MonsterEngine/Renderer/RHI/ISurface.h>
#include <MonsterEngine/WindowManager/Window.h>

#include <QuartzCore/CAMetalLayer.h>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API MetalSurface : public RHI::ISurface
	{
	public:
		MetalSurface(WindowManager::Window& window, CAMetalLayer* metalLayer);
		virtual ~MetalSurface();

		auto getWindowID() const { return m_WindowID; }

	private:
		std::uint32_t m_WindowID;
		CAMetalLayer* m_MetalLayer;
	};
} // namespace MonsterEngine::Renderer::Metal
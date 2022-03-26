#pragma once

#include "ISurface.h"

#include <memory>

namespace MonsterEngine::WindowManager
{
	class ME_MODULE_IMPORT Window;
}

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IDevice
	{
	public:
		virtual ~IDevice() = default;

		virtual std::unique_ptr<ISurface> newSurface(WindowManager::Window& window) = 0;

	private:
	};
} // namespace MonsterEngine::Renderer::RHI
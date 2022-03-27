#pragma once

#include "ISurface.h"
#include "RHINode.h"

#include <memory>
#include <string>

namespace MonsterEngine::WindowManager
{
	class ME_MODULE_IMPORT Window;
}

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IDevice : public RHINode
	{
	public:
		IDevice(const std::string& name);
		IDevice(std::string&& name);
		virtual ~IDevice() = default;

		virtual std::unique_ptr<ISurface> newSurface(WindowManager::Window& window) = 0;
	};
} // namespace MonsterEngine::Renderer::RHI
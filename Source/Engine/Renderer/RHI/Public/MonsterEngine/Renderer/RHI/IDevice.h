#pragma once

#include "RHINode.h"

#include <memory>
#include <string>

namespace MonsterEngine::WindowManager
{
	class ME_MODULE_IMPORT Window;
}

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IInstance;
	class MonsterEngine_Renderer_RHI_API ISwapchain;

	class MonsterEngine_Renderer_RHI_API IDevice : public RHINode
	{
	public:
		IDevice(const std::string& name, IInstance* instance);
		IDevice(std::string&& name, IInstance* instance);
		virtual ~IDevice() = default;

		virtual std::unique_ptr<ISwapchain> newSwapchain(WindowManager::Window& window) = 0;

		auto getInstance() const { return m_Instance; }

	private:
		IInstance* m_Instance;
	};
} // namespace MonsterEngine::Renderer::RHI
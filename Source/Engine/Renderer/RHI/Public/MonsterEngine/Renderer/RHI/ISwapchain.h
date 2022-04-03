#pragma once

#include "RHINode.h"

#include <string>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IDevice;

	class MonsterEngine_Renderer_RHI_API ISwapchain : public RHINode
	{
	public:
		ISwapchain(const std::string& name, IDevice* device);
		ISwapchain(std::string&& name, IDevice* device);
		virtual ~ISwapchain() = default;

		virtual bool begin() = 0;
		virtual void end()   = 0;

		auto getDevice() const { return m_Device; }

	private:
		IDevice* m_Device;
	};
} // namespace MonsterEngine::Renderer::RHI
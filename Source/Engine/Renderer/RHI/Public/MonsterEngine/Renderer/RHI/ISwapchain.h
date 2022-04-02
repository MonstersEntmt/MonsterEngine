#pragma once

#include "RHINode.h"

#include <string>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API ISwapchain : public RHINode
	{
	public:
		ISwapchain(const std::string& name);
		ISwapchain(std::string&& name);
		virtual ~ISwapchain() = default;

		virtual bool begin() = 0;
		virtual void end()   = 0;
	};
} // namespace MonsterEngine::Renderer::RHI
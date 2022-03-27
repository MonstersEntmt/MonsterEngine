#pragma once

#include "RHINode.h"

#include <string>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API ISurface : public RHINode
	{
	public:
		ISurface(const std::string& name);
		ISurface(std::string&& name);
		virtual ~ISurface() = default;
	};
} // namespace MonsterEngine::Renderer::RHI
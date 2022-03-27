#pragma once

#include "IDevice.h"
#include "RHINode.h"

#include <memory>
#include <string>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IInstance : public RHINode
	{
	public:
		IInstance(const std::string& name);
		IInstance(std::string&& name);
		virtual ~IInstance() = default;

		virtual std::unique_ptr<IDevice> findDevice() = 0;
	};
} // namespace MonsterEngine::Renderer::RHI
#pragma once

#include "IDevice.h"

#include <memory>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IInstance
	{
	public:
		virtual ~IInstance() = default;

		virtual std::unique_ptr<IDevice> findDevice() = 0;

	private:
	};
} // namespace MonsterEngine::Renderer::RHI
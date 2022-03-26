#pragma once

#include <MonsterEngine/Renderer/RHI/IRHI.h>

#include <memory>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API MetalRHI : public RHI::IRHI
	{
	public:
		MetalRHI();

		virtual bool isCompatible() override;

		virtual std::unique_ptr<RHI::IInstance> newInstance() override;

	private:
	};
} // namespace MonsterEngine::Renderer::Metal
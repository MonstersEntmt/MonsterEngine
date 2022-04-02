#pragma once

#include <MonsterEngine/Renderer/RHI/IInstance.h>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API MetalInstance : public RHI::IInstance
	{
	public:
		MetalInstance();
		virtual ~MetalInstance();

		virtual std::unique_ptr<RHI::IDevice> findDevice() override;
	};
} // namespace MonsterEngine::Renderer::Metal
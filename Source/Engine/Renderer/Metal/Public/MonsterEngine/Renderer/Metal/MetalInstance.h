#pragma once

#include <MonsterEngine/Renderer/RHI/IInstance.h>

namespace MonsterEngine::Renderer::Metal
{
	class MonsterEngine_Renderer_Metal_API MetalInstance : public RHI::IInstance
	{
	public:
		MetalInstance();
		virtual ~MetalInstance();

	private:
	};
} // namespace MonsterEngine::Renderer::Metal
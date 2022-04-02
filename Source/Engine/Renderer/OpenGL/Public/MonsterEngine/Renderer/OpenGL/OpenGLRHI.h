#pragma once

#include <MonsterEngine/Renderer/RHI/IRHI.h>

namespace MonsterEngine::Renderer::OpenGL
{
	class MonsterEngine_Renderer_OpenGL_API OpenGLRHI : public RHI::IRHI
	{
	public:
		OpenGLRHI();

		virtual bool isCompatible() override;

		virtual void setGLFWOptions(WindowManager::Window& window) override;

		virtual std::unique_ptr<RHI::IInstance> newInstance() override;
	};
} // namespace MonsterEngine::Renderer::OpenGL
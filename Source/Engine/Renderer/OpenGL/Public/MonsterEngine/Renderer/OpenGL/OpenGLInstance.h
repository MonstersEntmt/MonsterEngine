#pragma once

#include <MonsterEngine/Renderer/RHI/IInstance.h>

namespace MonsterEngine::Renderer::OpenGL
{
	class MonsterEngine_Renderer_OpenGL_API OpenGLInstance : public RHI::IInstance
	{
	public:
		OpenGLInstance(const std::string& name);
		OpenGLInstance(std::string&& name);

		virtual std::unique_ptr<RHI::IDevice> findDevice() override;
	};
} // namespace MonsterEngine::Renderer::OpenGL
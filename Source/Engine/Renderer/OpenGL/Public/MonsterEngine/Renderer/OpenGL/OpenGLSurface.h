#pragma once

#include <MonsterEngine/Renderer/RHI/ISurface.h>

namespace MonsterEngine::Renderer::OpenGL
{
	class MonsterEngine_Renderer_OpenGL_API OpenGLDevice;

	class MonsterEngine_Renderer_OpenGL_API OpenGLSurface : public RHI::ISurface
	{
	public:
		OpenGLSurface(const std::string& name, OpenGLDevice* device);
		OpenGLSurface(std::string&& name, OpenGLDevice* device);

		auto getDevice() const { return m_Device; }

	private:
		OpenGLDevice* m_Device;
	};
} // namespace MonsterEngine::Renderer::OpenGL
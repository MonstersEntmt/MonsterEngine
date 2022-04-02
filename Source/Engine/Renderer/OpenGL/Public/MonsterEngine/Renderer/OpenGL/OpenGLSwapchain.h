#pragma once

#include <MonsterEngine/Renderer/RHI/ISwapchain.h>

namespace MonsterEngine::Renderer::OpenGL
{
	class MonsterEngine_Renderer_OpenGL_API OpenGLDevice;

	class MonsterEngine_Renderer_OpenGL_API OpenGLSwapchain : public RHI::ISwapchain
	{
	public:
		OpenGLSwapchain(const std::string& name, OpenGLDevice* device);
		OpenGLSwapchain(std::string&& name, OpenGLDevice* device);

		auto getDevice() const { return m_Device; }

	private:
		OpenGLDevice* m_Device;
	};
} // namespace MonsterEngine::Renderer::OpenGL
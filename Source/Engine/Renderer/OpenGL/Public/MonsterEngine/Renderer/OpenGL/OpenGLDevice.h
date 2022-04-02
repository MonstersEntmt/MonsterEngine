#pragma once

#include <MonsterEngine/Renderer/RHI/IDevice.h>

namespace MonsterEngine::Renderer::OpenGL
{
	class MonsterEngine_Renderer_OpenGL_API OpenGLInstance;

	class MonsterEngine_Renderer_OpenGL_API OpenGLDevice : public RHI::IDevice
	{
	public:
		OpenGLDevice(const std::string& name, OpenGLInstance* instance);
		OpenGLDevice(std::string&& name, OpenGLInstance* instance);

		virtual std::unique_ptr<RHI::ISwapchain> newSwapchain(WindowManager::Window& window) override;

		auto getInstance() const { return m_Instance; }

	private:
		OpenGLInstance* m_Instance;
	};
} // namespace MonsterEngine::Renderer::OpenGL
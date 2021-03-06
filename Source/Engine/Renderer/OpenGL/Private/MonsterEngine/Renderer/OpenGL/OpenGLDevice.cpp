#include "MonsterEngine/Renderer/OpenGL/OpenGLDevice.h"
#include "MonsterEngine/Renderer/OpenGL/OpenGLSwapchain.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <utility>

namespace MonsterEngine::Renderer::OpenGL
{
	OpenGLDevice::OpenGLDevice(const std::string& name, OpenGLInstance* instance)
	    : RHI::IDevice(name), m_Instance(instance) {}

	OpenGLDevice::OpenGLDevice(std::string&& name, OpenGLInstance* instance)
	    : RHI::IDevice(std::move(name)), m_Instance(instance) {}

	std::unique_ptr<RHI::ISwapchain> OpenGLDevice::newSwapchain(WindowManager::Window& window)
	{
		return std::make_unique<OpenGLSwapchain>("Window " + std::to_string(window.getId()), this);
	}
} // namespace MonsterEngine::Renderer::OpenGL
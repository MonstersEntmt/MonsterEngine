#include "MonsterEngine/Renderer/OpenGL/OpenGLSwapchain.h"

#include <utility>

namespace MonsterEngine::Renderer::OpenGL
{
	OpenGLSwapchain::OpenGLSwapchain(const std::string& name, OpenGLDevice* device)
	    : RHI::ISwapchain(name), m_Device(device) {}

	OpenGLSwapchain::OpenGLSwapchain(std::string&& name, OpenGLDevice* device)
	    : RHI::ISwapchain(std::move(name)), m_Device(device) {}
} // namespace MonsterEngine::Renderer::OpenGL
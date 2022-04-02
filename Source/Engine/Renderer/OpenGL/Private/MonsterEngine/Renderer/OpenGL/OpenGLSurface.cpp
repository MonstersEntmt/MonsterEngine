#include "MonsterEngine/Renderer/OpenGL/OpenGLSurface.h"

#include <utility>

namespace MonsterEngine::Renderer::OpenGL
{
	OpenGLSurface::OpenGLSurface(const std::string& name, OpenGLDevice* device)
	    : RHI::ISurface(name), m_Device(device) {}

	OpenGLSurface::OpenGLSurface(std::string&& name, OpenGLDevice* device)
	    : RHI::ISurface(std::move(name)), m_Device(device) {}
} // namespace MonsterEngine::Renderer::OpenGL
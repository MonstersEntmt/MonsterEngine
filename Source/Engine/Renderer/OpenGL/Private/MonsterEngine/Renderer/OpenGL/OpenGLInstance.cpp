#include "MonsterEngine/Renderer/OpenGL/OpenGLInstance.h"
#include "MonsterEngine/Renderer/OpenGL/OpenGLDevice.h"

#include "Platforms/OpenGL/glad.h"

#include "MonsterEngine/Logger/Logger.h"

#include <utility>

namespace MonsterEngine::Renderer::OpenGL
{
	OpenGLInstance::OpenGLInstance(const std::string& name)
	    : IInstance(name) {}

	OpenGLInstance::OpenGLInstance(std::string&& name)
	    : IInstance(std::move(name)) {}

	std::unique_ptr<RHI::IDevice> OpenGLInstance::findDevice()
	{
		std::string vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		Logger::Trace("Found OpenGL device '{} {}'", vendor, renderer);
		return std::make_unique<OpenGLDevice>("Device " + vendor + ' ' + renderer, this);
	}
} // namespace MonsterEngine::Renderer::OpenGL
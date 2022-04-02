#include "MonsterEngine/Renderer/OpenGL/OpenGLInstance.h"
#include "MonsterEngine/Renderer/OpenGL/OpenGLDevice.h"

#include <utility>

namespace MonsterEngine::Renderer::OpenGL
{
	OpenGLInstance::OpenGLInstance(const std::string& name)
	    : IInstance(name) {}

	OpenGLInstance::OpenGLInstance(std::string&& name)
	    : IInstance(std::move(name)) {}

	std::unique_ptr<RHI::IDevice> OpenGLInstance::findDevice()
	{
		return std::make_unique<OpenGLDevice>("Device", this);
	}
} // namespace MonsterEngine::Renderer::OpenGL
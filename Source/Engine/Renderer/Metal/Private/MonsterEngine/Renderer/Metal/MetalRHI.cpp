#include "MonsterEngine/Renderer/Metal/MetalRHI.h"
#include "MonsterEngine/Renderer/Metal/MetalInstance.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <GLFW/glfw3.h>

namespace MonsterEngine::Renderer::Metal
{
	MetalRHI::MetalRHI()
	    : RHI::IRHI("Metal") {}

	bool MetalRHI::isCompatible()
	{
		return true;
	}

	void MetalRHI::setGLFWOptions(WindowManager::Window& window)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	std::unique_ptr<RHI::IInstance> MetalRHI::newInstance()
	{
		return std::make_unique<MetalInstance>();
	}
} // namespace MonsterEngine::Renderer::Metal
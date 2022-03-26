#include "MonsterEngine/Renderer/Vulkan/VulkanRHI.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"

#include <GLFW/glfw3.h>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanRHI::VulkanRHI()
	    : RHI::IRHI("Vulkan") {}

	bool VulkanRHI::isCompatible()
	{
		return glfwVulkanSupported();
	}

	void VulkanRHI::setGLFWOptions(WindowManager::Window& window)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	std::unique_ptr<RHI::IInstance> VulkanRHI::newInstance()
	{
		return std::make_unique<VulkanInstance>("MonsterGame", Version { 0, 1, 0 });
	}
} // namespace MonsterEngine::Renderer::Vulkan
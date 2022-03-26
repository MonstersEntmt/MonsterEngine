#include "MonsterEngine/Renderer/Metal/MetalDevice.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

namespace MonsterEngine::Renderer::Metal
{
	MetalDevice::MetalDevice(MTL::Device* device)
	    : m_Device(device) {}

	MetalDevice::~MetalDevice()
	{
		m_Device->release();
	}

	std::unique_ptr<RHI::ISurface> MetalDevice::newSurface(WindowManager::Window& window)
	{
		CA::MetalLayer* metalLayer;

		id nswindow = glfwGetCocoaWindow(window.getNative());

		return std::make_unique<MetalSurface>(window, metalLayer);
	}
} // namespace MonsterEngine::Renderer::Metal
#include "MonsterEngine/Renderer/Metal/MetalDevice.h"
#include "MonsterEngine/Renderer/Metal/MetalSwapchain.h"

#include <MonsterEngine/WindowManager/Window.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include <AppKit/AppKit.h>

namespace MonsterEngine::Renderer::Metal
{
	MetalDevice::MetalDevice(MTL::Device* device)
	    : m_Device(device) {}

	MetalDevice::~MetalDevice()
	{
		m_Device->release();
	}

	std::unique_ptr<RHI::ISwapchain> MetalDevice::newSurface(WindowManager::Window& window)
	{
		NSWindow* nswindow              = glfwGetCocoaWindow(window.getNative());
		nswindow.contentView.layer      = [CAMetalLayer layer];
		nswindow.contentView.wantsLayer = YES;
		CAMetalLayer* metalLayer        = (CAMetalLayer*) nswindow.contentView.layer;
		metalLayer.device               = (id<MTLDevice>) m_Device;
		metalLayer.pixelFormat          = MTLPixelFormatBGRA8Unorm;
		metalLayer.frame.size           = { static_cast<CGFloat>(window.getWidth()), static_cast<CGFloat>(window.getHeight()) };
		return std::make_unique<MetalSwapchain>(window, metalLayer);
	}
} // namespace MonsterEngine::Renderer::Metal
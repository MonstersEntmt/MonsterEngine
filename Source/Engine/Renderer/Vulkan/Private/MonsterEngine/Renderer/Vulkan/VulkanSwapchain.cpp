#include "MonsterEngine/Renderer/Vulkan/VulkanSwapchain.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanDevice.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanHelper.h"
#include "MonsterEngine/Renderer/Vulkan/VulkanInstance.h"

#include <MonsterEngine/Logger/Logger.h>
#include <MonsterEngine/WindowManager/Window.h>
#include <MonsterEngine/WindowManager/WindowManager.h>

#include <GLFW/glfw3.h>

#include <utility>

namespace MonsterEngine::Renderer::Vulkan
{
	VulkanSwapchain::VulkanSwapchain(const std::string& name, VulkanDevice* device, WindowManager::Window& window)
	    : ISwapchain(name, device), m_WindowId(window.getId())
	{
		create();
	}

	VulkanSwapchain::VulkanSwapchain(std::string&& name, VulkanDevice* device, WindowManager::Window& window)
	    : ISwapchain(std::move(name), device), m_WindowId(window.getId())
	{
		create();
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		VulkanSwapchain::destroy();
	}

	bool VulkanSwapchain::begin()
	{
		auto device = getVulkanDevice();

		// TODO(MarcasRealAccount): Replace nullptrs with syncronizations
		std::uint32_t imageIndex;
		if (VkCall({}, vkAcquireNextImageKHR(device->getHandle(), m_Swapchain, ~0ULL, nullptr, nullptr, &imageIndex),
		           "Failed to acquire next Swapchain image for window {}", m_WindowId) == VK_SUBOPTIMAL_KHR)
		{
			recreate();
			return false;
		}

		VkRect2D viewport { { 0, 0 }, { m_Width, m_Height } };

		VkClearValue colorClear {};
		colorClear.color = { 0.1f, 0.1f, 0.1f, 1.0f };

		VkClearValue depthClear {};
		depthClear.depthStencil = { 0.0f };

		VkClearValue stencilClear {};
		stencilClear.depthStencil = { 0 };

		VkRenderingAttachmentInfo colorAttachment {};
		colorAttachment.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorAttachment.pNext              = nullptr;
		colorAttachment.imageView          = m_ColorViews[imageIndex];
		colorAttachment.imageLayout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachment.resolveMode        = VK_RESOLVE_MODE_NONE;
		colorAttachment.resolveImageView   = nullptr;
		colorAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.clearValue         = colorClear;

		VkRenderingAttachmentInfo depthAttachment {};
		depthAttachment.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		depthAttachment.pNext              = nullptr;
		depthAttachment.imageView          = m_DepthViews[imageIndex];
		depthAttachment.imageLayout        = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
		depthAttachment.resolveMode        = VK_RESOLVE_MODE_NONE;
		depthAttachment.resolveImageView   = nullptr;
		depthAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachment.clearValue         = depthClear;

		VkRenderingAttachmentInfo stencilAttachment {};
		stencilAttachment.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		stencilAttachment.pNext              = nullptr;
		stencilAttachment.imageView          = m_StencilViews[imageIndex];
		stencilAttachment.imageLayout        = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
		stencilAttachment.resolveMode        = VK_RESOLVE_MODE_NONE;
		stencilAttachment.resolveImageView   = nullptr;
		stencilAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		stencilAttachment.loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR;
		stencilAttachment.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
		stencilAttachment.clearValue         = depthClear;

		VkRenderingInfo renderingInfo {};
		renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.pNext                = nullptr;
		renderingInfo.flags                = 0;
		renderingInfo.renderArea           = viewport;
		renderingInfo.layerCount           = 1;
		renderingInfo.viewMask             = 0;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachments    = &colorAttachment;
		renderingInfo.pDepthAttachment     = &depthAttachment;
		renderingInfo.pStencilAttachment   = &stencilAttachment;

		// TODO(MarcasRealAccount): Replace nullptr with a command buffer from the device
		vkCmdBeginRendering(nullptr, &renderingInfo);

		return true;
	}

	void VulkanSwapchain::end()
	{
		// TODO(MarcasRealAccount): Replace nullptr with a command buffer from the device
		vkCmdEndRendering(nullptr);
	}

	VulkanDevice* VulkanSwapchain::getVulkanDevice() const
	{
		return static_cast<VulkanDevice*>(getDevice());
	}

	void VulkanSwapchain::create()
	{
		auto logger = Logger("Vulkan");

		auto vkDevice       = getVulkanDevice();
		auto device         = vkDevice->getHandle();
		auto instance       = vkDevice->getVulkanInstance();
		auto physicalDevice = vkDevice->getPhysicalDevice();

		auto window = WindowManager::WindowManager::Get().getWindow(m_WindowId);
		if (!window)
			logger.exception({}, "Invalid window {} used for Swapchain!", m_WindowId);

		VkCall({}, glfwCreateWindowSurface(instance->getHandle(), window->getNative(), nullptr, &m_Surface),
		       "Failed to create VkSurfaceKHR for window {}", m_WindowId);

		VkSurfaceCapabilitiesKHR surfaceCaps {};
		VkCall({}, vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &surfaceCaps),
		       "Failed to query surface capabilities for window {}", m_WindowId);

		std::uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr);
		if (!formatCount)
			logger.exception({}, "Surface for window {} doesn't support any formats!", m_WindowId);

		std::vector<VkSurfaceFormatKHR> formats { formatCount };
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, formats.data());

		m_Format = formats[0];
		for (auto fmt : formats)
			logger.trace("Window {} supports format {} and colorspace {}", m_WindowId, fmt.format, fmt.colorSpace);

		std::uint32_t presentCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentCount, nullptr);
		if (!presentCount)
			logger.exception({}, "Surface for window {} doesn't support any present modes!", m_WindowId);

		std::vector<VkPresentModeKHR> presentModes { presentCount };
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentCount, presentModes.data());

		m_PresentMode = presentModes[0];
		for (auto presentMode : presentModes)
			logger.trace("Window {} supports present mode {}", m_WindowId, presentMode);

		logger.trace("Window {} uses format {} and colorspace {} and present mode {}", m_WindowId, m_Format.format, m_Format.colorSpace, m_PresentMode);
		std::uint32_t imageCount = std::min(surfaceCaps.minImageCount + 1, surfaceCaps.maxImageCount);
		VkExtent2D    extent     = surfaceCaps.currentExtent;

		m_Width  = extent.width;
		m_Height = extent.height;

		VkSwapchainCreateInfoKHR createInfo {};
		createInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext                 = nullptr;
		createInfo.flags                 = 0;
		createInfo.surface               = m_Surface;
		createInfo.minImageCount         = imageCount;
		createInfo.imageFormat           = m_Format.format;
		createInfo.imageColorSpace       = m_Format.colorSpace;
		createInfo.imageExtent           = extent;
		createInfo.imageArrayLayers      = 1;
		createInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices   = nullptr;
		createInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		createInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode           = m_PresentMode;
		createInfo.clipped               = false;
		createInfo.oldSwapchain          = nullptr;

		VkCall({}, vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_Swapchain),
		       "Failed to create VkSwapchainKHR for window {}!", m_WindowId);

		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, nullptr);
		m_ColorImages.resize(imageCount);
		m_DepthStencilImages.resize(imageCount);
		m_ColorViews.resize(imageCount);
		m_DepthViews.resize(imageCount);
		m_StencilViews.resize(imageCount);

		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, m_ColorImages.data());

		for (std::size_t i = 0; i < imageCount; ++i)
		{
			VkImageViewCreateInfo viewCreateInfo {};
			viewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext                           = nullptr;
			viewCreateInfo.flags                           = 0;
			viewCreateInfo.image                           = m_ColorImages[i];
			viewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format                          = m_Format.format;
			viewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel   = 0;
			viewCreateInfo.subresourceRange.levelCount     = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount     = 1;

			VkImageView imageView;
			VkCall({}, vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView),
			       "Failed to create VkImageView for window {} color image {}", m_WindowId, i);
			m_ColorViews[i] = imageView;

			VkImageCreateInfo depthStencilImageCreateInfo {};
			depthStencilImageCreateInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			depthStencilImageCreateInfo.pNext                 = nullptr;
			depthStencilImageCreateInfo.flags                 = 0;
			depthStencilImageCreateInfo.imageType             = VK_IMAGE_TYPE_2D;
			depthStencilImageCreateInfo.format                = VK_FORMAT_D32_SFLOAT_S8_UINT;
			depthStencilImageCreateInfo.extent                = { m_Width, m_Height, 0 };
			depthStencilImageCreateInfo.mipLevels             = 1;
			depthStencilImageCreateInfo.arrayLayers           = 1;
			depthStencilImageCreateInfo.samples               = VK_SAMPLE_COUNT_1_BIT;
			depthStencilImageCreateInfo.tiling                = VK_IMAGE_TILING_OPTIMAL;
			depthStencilImageCreateInfo.usage                 = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			depthStencilImageCreateInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
			depthStencilImageCreateInfo.queueFamilyIndexCount = 0;
			depthStencilImageCreateInfo.pQueueFamilyIndices   = nullptr;
			depthStencilImageCreateInfo.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;

			VkImage depthStencilImage;
			VkCall({}, vkCreateImage(device, &depthStencilImageCreateInfo, nullptr, &depthStencilImage),
			       "Failed to create VkImage for window {} depth stencil image {}", m_WindowId, i);
			m_DepthStencilImages[i] = depthStencilImage;

			viewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext                           = nullptr;
			viewCreateInfo.flags                           = 0;
			viewCreateInfo.image                           = m_DepthStencilImages[i];
			viewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format                          = VK_FORMAT_D32_SFLOAT_S8_UINT;
			viewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel   = 0;
			viewCreateInfo.subresourceRange.levelCount     = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount     = 1;

			VkCall({}, vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView),
			       "Failed to create VkImageView for window {} depth image {}", m_WindowId, i);
			m_DepthViews[i] = imageView;

			viewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext                           = nullptr;
			viewCreateInfo.flags                           = 0;
			viewCreateInfo.image                           = m_DepthStencilImages[i];
			viewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format                          = VK_FORMAT_D32_SFLOAT_S8_UINT;
			viewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_STENCIL_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel   = 0;
			viewCreateInfo.subresourceRange.levelCount     = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount     = 1;

			VkCall({}, vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView),
			       "Failed to create VkImageView for window {} stencil image {}", m_WindowId, i);
			m_StencilViews[i] = imageView;
		}
	}

	void VulkanSwapchain::recreate()
	{
		auto logger = Logger("Vulkan");

		auto vkDevice       = getVulkanDevice();
		auto device         = vkDevice->getHandle();
		auto physicalDevice = vkDevice->getPhysicalDevice();

		for (auto view : m_ColorViews)
			vkDestroyImageView(device, view, nullptr);

		for (auto view : m_DepthViews)
			vkDestroyImageView(device, view, nullptr);

		for (auto view : m_StencilViews)
			vkDestroyImageView(device, view, nullptr);

		for (auto image : m_DepthStencilImages)
			vkDestroyImage(device, image, nullptr);

		m_ColorViews.clear();
		m_DepthViews.clear();
		m_StencilViews.clear();
		m_DepthStencilImages.clear();

		VkSurfaceCapabilitiesKHR surfaceCaps {};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &surfaceCaps);

		std::uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr);
		if (!formatCount)
			logger.exception({}, "Surface for window {} doesn't support any formats!", m_WindowId);

		std::vector<VkSurfaceFormatKHR> formats { formatCount };
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, formats.data());

		std::uint32_t presentCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentCount, nullptr);
		if (!presentCount)
			logger.exception({}, "Surface for window {} doesn't support any present modes!", m_WindowId);

		std::vector<VkPresentModeKHR> presentModes { presentCount };
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentCount, presentModes.data());

		m_Format                 = formats[0];
		m_PresentMode            = presentModes[0];
		std::uint32_t imageCount = std::min(surfaceCaps.minImageCount + 1, surfaceCaps.maxImageCount);
		VkExtent2D    extent     = surfaceCaps.currentExtent;

		m_Width  = extent.width;
		m_Height = extent.height;

		VkSwapchainCreateInfoKHR createInfo {};
		createInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext                 = nullptr;
		createInfo.flags                 = 0;
		createInfo.surface               = m_Surface;
		createInfo.minImageCount         = imageCount;
		createInfo.imageFormat           = m_Format.format;
		createInfo.imageColorSpace       = m_Format.colorSpace;
		createInfo.imageExtent           = extent;
		createInfo.imageArrayLayers      = 1;
		createInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices   = nullptr;
		createInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		createInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode           = m_PresentMode;
		createInfo.clipped               = false;
		createInfo.oldSwapchain          = m_Swapchain;

		VkCall({}, vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_Swapchain),
		       "Failed to create VkSwapchainKHR for window {}!", m_WindowId);

		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, nullptr);
		m_ColorImages.resize(imageCount);
		m_DepthStencilImages.resize(imageCount);
		m_ColorViews.resize(imageCount);
		m_DepthViews.resize(imageCount);
		m_StencilViews.resize(imageCount);

		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, m_ColorImages.data());

		for (std::size_t i = 0; i < imageCount; ++i)
		{
			VkImageViewCreateInfo viewCreateInfo {};
			viewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext                           = nullptr;
			viewCreateInfo.flags                           = 0;
			viewCreateInfo.image                           = m_ColorImages[i];
			viewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format                          = m_Format.format;
			viewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel   = 0;
			viewCreateInfo.subresourceRange.levelCount     = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount     = 1;

			VkImageView imageView;
			VkCall({}, vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView),
			       "Failed to create VkImageView for window {} color image {}", m_WindowId, i);
			m_ColorViews[i] = imageView;

			VkImageCreateInfo depthStencilImageCreateInfo {};
			depthStencilImageCreateInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			depthStencilImageCreateInfo.pNext                 = nullptr;
			depthStencilImageCreateInfo.flags                 = 0;
			depthStencilImageCreateInfo.imageType             = VK_IMAGE_TYPE_2D;
			depthStencilImageCreateInfo.format                = VK_FORMAT_D32_SFLOAT_S8_UINT;
			depthStencilImageCreateInfo.extent                = { m_Width, m_Height, 0 };
			depthStencilImageCreateInfo.mipLevels             = 1;
			depthStencilImageCreateInfo.arrayLayers           = 1;
			depthStencilImageCreateInfo.samples               = VK_SAMPLE_COUNT_1_BIT;
			depthStencilImageCreateInfo.tiling                = VK_IMAGE_TILING_OPTIMAL;
			depthStencilImageCreateInfo.usage                 = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			depthStencilImageCreateInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
			depthStencilImageCreateInfo.queueFamilyIndexCount = 0;
			depthStencilImageCreateInfo.pQueueFamilyIndices   = nullptr;
			depthStencilImageCreateInfo.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;

			VkImage depthStencilImage;
			VkCall({}, vkCreateImage(device, &depthStencilImageCreateInfo, nullptr, &depthStencilImage),
			       "Failed to create VkImage for window {} depth stencil image {}", m_WindowId, i);
			m_DepthStencilImages[i] = depthStencilImage;

			viewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext                           = nullptr;
			viewCreateInfo.flags                           = 0;
			viewCreateInfo.image                           = m_DepthStencilImages[i];
			viewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format                          = VK_FORMAT_D32_SFLOAT_S8_UINT;
			viewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel   = 0;
			viewCreateInfo.subresourceRange.levelCount     = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount     = 1;

			VkCall({}, vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView),
			       "Failed to create VkImageView for window {} depth image {}", m_WindowId, i);
			m_DepthViews[i] = imageView;

			viewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.pNext                           = nullptr;
			viewCreateInfo.flags                           = 0;
			viewCreateInfo.image                           = m_DepthStencilImages[i];
			viewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format                          = VK_FORMAT_D32_SFLOAT_S8_UINT;
			viewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_STENCIL_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel   = 0;
			viewCreateInfo.subresourceRange.levelCount     = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount     = 1;

			VkCall({}, vkCreateImageView(device, &viewCreateInfo, nullptr, &imageView),
			       "Failed to create VkImageView for window {} stencil image {}", m_WindowId, i);
			m_StencilViews[i] = imageView;
		}
	}

	void VulkanSwapchain::destroy()
	{
		auto vkDevice = getVulkanDevice();
		auto device   = vkDevice->getHandle();

		if (m_Swapchain)
		{
			for (auto view : m_ColorViews)
				vkDestroyImageView(device, view, nullptr);

			for (auto view : m_DepthViews)
				vkDestroyImageView(device, view, nullptr);

			for (auto view : m_StencilViews)
				vkDestroyImageView(device, view, nullptr);

			for (auto image : m_DepthStencilImages)
				vkDestroyImage(device, image, nullptr);

			m_ColorViews.clear();
			m_DepthViews.clear();
			m_StencilViews.clear();
			m_DepthStencilImages.clear();

			vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
		}

		if (m_Surface)
			vkDestroySurfaceKHR(vkDevice->getVulkanInstance()->getHandle(), m_Surface, nullptr);
		m_Surface = nullptr;
	}
} // namespace MonsterEngine::Renderer::Vulkan
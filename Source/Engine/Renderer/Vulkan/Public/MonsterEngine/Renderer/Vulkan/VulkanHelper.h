#pragma once

#include <MonsterEngine/Logger/Logger.h>

#include <vulkan/vulkan.h>

#include <string>

namespace MonsterEngine::Renderer::Vulkan
{
	MonsterEngine_Renderer_Vulkan_API const char* VulkanResultToString(VkResult result);

	template <class... Args>
	VkResult VkCall(Logger::Src src, VkResult result, fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (result < VK_SUCCESS)
		{
			if constexpr (sizeof...(Args) > 0)
			{
				std::string msg = fmt::format<Args...>(fmt, std::forward<Args>(args)...);
				Logger("Vulkan").exception(src, "Returned '{}': {}", VulkanResultToString(result), msg);
			}
			else
			{
				Logger("Vulkan").exception(src, "Returned '{}'", VulkanResultToString(result));
			}
		}
		return result;
	}

	template <class... Args>
	VkResult VkCall(VkResult result, fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (result < VK_SUCCESS)
		{
			if constexpr (sizeof...(Args) > 0)
			{
				std::string msg = fmt::format<Args...>(fmt, std::forward<Args>(args)...);
				Logger("Vulkan").exception("Returned '{}': {}", VulkanResultToString(result), msg);
			}
			else
			{
				Logger("Vulkan").exception("Returned '{}'", VulkanResultToString(result));
			}
		}
		return result;
	}

	template <class T>
	VkResult VkCall(Logger::Src src, VkResult result, const T& msg)
	{
		if (result < VK_SUCCESS)
			Logger("Vulkan").exception(src, "Returned '{}': {}", VulkanResultToString(result), msg);
		return result;
	}

	template <class T>
	VkResult VkCall(VkResult result, const T& msg)
	{
		if (result < VK_SUCCESS)
			Logger("Vulkan").exception("Returned '{}': {}", VulkanResultToString(result), msg);
		return result;
	}
} // namespace MonsterEngine::Renderer::Vulkan
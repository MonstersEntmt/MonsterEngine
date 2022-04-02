#include "MonsterEngine/Renderer/OpenGL/OpenGLRHI.h"
#include "MonsterEngine/Renderer/OpenGL/OpenGLInstance.h"

#include "Platforms/OpenGL/glad.h"

#include <MonsterEngine/WindowManager/WindowManager.h>

#include <GLFW/glfw3.h>

namespace MonsterEngine::Renderer::OpenGL
{
	OpenGLRHI::OpenGLRHI()
	    : IRHI("OpenGL") {}

	bool isCompatible()
	{
		WindowManager::WindowManager::Get();

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		GLFWwindow* window = glfwCreateWindow(256, 256, nullptr, nullptr, nullptr);
		if (!window)
			return false;

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(&glfwGetProcAddress)))
		{
			glfwDestroyWindow(window);
			return false;
		}

		if (GLVersion.major < 4 || (GLVersion.major == 4 && GLVersion.minor < 6))
		{
			glfwDestroyWindow(window);
			return false;
		}

		glfwDestroyWindow(window);
		return true;
	}

	void setGLFWOptions(WindowManager::Window& window)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	}

	std::unique_ptr<RHI::IInstance> newInstance()
	{
		return std::make_unique<OpenGLInstance>("MonsterGame");
	}
} // namespace MonsterEngine::Renderer::OpenGL
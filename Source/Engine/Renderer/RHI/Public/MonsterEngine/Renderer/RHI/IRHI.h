#pragma once

#include "IInstance.h"

#include <memory>
#include <string>

namespace MonsterEngine::WindowManager
{
	class ME_MODULE_IMPORT Window;
}

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IRHI
	{
	public:
		IRHI(const std::string& name);
		IRHI(std::string&& name);
		virtual ~IRHI() = default;

		auto& getName() const { return m_Name; }

		virtual bool isCompatible() = 0;

		virtual void setGLFWOptions(WindowManager::Window& window) = 0;

		virtual std::unique_ptr<IInstance> newInstance() = 0;

	private:
		std::string m_Name;
	};
} // namespace MonsterEngine::Renderer::RHI
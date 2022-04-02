#pragma once

#include "IDevice.h"
#include "IInstance.h"
#include "RHINode.h"

#include <memory>
#include <string>
#include <vector>

namespace MonsterEngine::WindowManager
{
	class ME_MODULE_IMPORT Window;
}

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API IRHI : public RHINode
	{
	public:
		IRHI(const std::string& name);
		IRHI(std::string&& name);
		virtual ~IRHI() = default;

		void init();

		virtual bool isCompatible() = 0;

		virtual void setGLFWOptions(WindowManager::Window& window) = 0;

		virtual std::unique_ptr<IInstance> newInstance() = 0;

		auto getMainInstance() const { return m_Instance.get(); }
		auto getMainDevice() const { return m_Device.get(); }

	private:
		virtual void destroy() override;

	private:
		std::string m_Name;

		std::unique_ptr<RHI::IInstance> m_Instance;
		std::unique_ptr<RHI::IDevice>   m_Device;
	};
} // namespace MonsterEngine::Renderer::RHI
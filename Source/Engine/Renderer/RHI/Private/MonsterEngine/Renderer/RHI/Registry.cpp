#include "MonsterEngine/Renderer/RHI/Registry.h"
#include "MonsterEngine/Renderer/RHI/IRHI.h"

#include <MonsterEngine/Logger/Logger.h>

namespace MonsterEngine::Renderer::RHI
{
	Registry& Registry::Get()
	{
		static Registry* s_Instance = new Registry();
		return *s_Instance;
	}

	void Registry::Destroy()
	{
		delete &Get();
	}

	IRHI* Registry::registerRHI(std::unique_ptr<IRHI>&& rhi)
	{
		Logger::Trace("Registered RHI '{}'", rhi->getName());
		return m_RHIs.emplace_back(std::move(rhi)).get();
	}

	void Registry::unregisterRHI(IRHI* rhi)
	{
		auto itr = std::find_if(m_RHIs.begin(), m_RHIs.end(),
		                        [rhi](const std::unique_ptr<IRHI>& r) -> bool
		                        {
			                        return r.get() == rhi;
		                        });
		if (itr != m_RHIs.end())
		{
			Logger::Trace("Unregistered RHI '{}'", rhi->getName());
			m_RHIs.erase(itr);
		}
	}

	IRHI* Registry::getRHI(std::string_view name)
	{
		auto itr = std::find_if(m_RHIs.begin(), m_RHIs.end(),
		                        [name](const std::unique_ptr<IRHI>& r) -> bool
		                        {
			                        return r->getName() == name;
		                        });
		return itr != m_RHIs.end() ? itr->get() : nullptr;
	}

	const IRHI* Registry::getRHI(std::string_view name) const
	{
		auto itr = std::find_if(m_RHIs.begin(), m_RHIs.end(),
		                        [name](const std::unique_ptr<IRHI>& r) -> bool
		                        {
			                        return r->getName() == name;
		                        });
		return itr != m_RHIs.end() ? itr->get() : nullptr;
	}

	void Registry::selectRHI(std::string_view name)
	{
		m_SelectedRHI = getRHI(name);
	}
} // namespace MonsterEngine::Renderer::RHI
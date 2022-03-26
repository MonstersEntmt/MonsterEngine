#pragma once

#include "IRHI.h"

#include <cstdint>

#include <memory>
#include <string_view>
#include <vector>

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API Registry
	{
	public:
		static Registry& Get();
		static void      Destroy();

	public:
		IRHI* registerRHI(std::unique_ptr<IRHI>&& rhi);
		void  unregisterRHI(IRHI* rhi);

		IRHI*       getRHI(std::string_view name);
		const IRHI* getRHI(std::string_view name) const;
		auto&       getRHIs() const { return m_RHIs; }

	private:
		Registry()                = default;
		Registry(const Registry&) = delete;
		Registry(Registry&&)      = delete;
		Registry& operator=(const Registry&) = delete;
		Registry& operator=(Registry&&) = delete;
		~Registry()                     = default;

	private:
		std::vector<std::unique_ptr<IRHI>> m_RHIs;
	};
} // namespace MonsterEngine::Renderer::RHI
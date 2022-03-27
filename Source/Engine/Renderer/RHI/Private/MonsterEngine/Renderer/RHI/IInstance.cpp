#include "MonsterEngine/Renderer/RHI/IInstance.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	IInstance::IInstance(const std::string& name)
	    : RHINode(name) {}

	IInstance::IInstance(std::string&& name)
	    : RHINode(std::move(name)) {}
} // namespace MonsterEngine::Renderer::RHI
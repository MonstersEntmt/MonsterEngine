#include "MonsterEngine/Renderer/RHI/ISurface.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	ISurface::ISurface(const std::string& name)
	    : RHINode(name) {}

	ISurface::ISurface(std::string&& name)
	    : RHINode(std::move(name)) {}
} // namespace MonsterEngine::Renderer::RHI
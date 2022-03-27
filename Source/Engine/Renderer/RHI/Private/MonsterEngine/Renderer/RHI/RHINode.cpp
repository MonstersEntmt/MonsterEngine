#include "MonsterEngine/Renderer/RHI/RHINode.h"

#include <utility>

namespace MonsterEngine::Renderer::RHI
{
	RHINode::RHINode(const std::string& name)
	    : Node(name) {}

	RHINode::RHINode(std::string&& name)
	    : Node(std::move(name)) {}

	void RHINode::onDetached(Node* parent)
	{
		detachChildren();
		destroy();
	}
} // namespace MonsterEngine::Renderer::RHI
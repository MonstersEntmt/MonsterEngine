#pragma once

#include "Node.h"

namespace MonsterEngine::Renderer::RHI
{
	class MonsterEngine_Renderer_RHI_API RHINode : public Node
	{
	public:
		RHINode(const std::string& name);
		RHINode(std::string&& name);

	private:
		virtual void onDetached(Node* parent) override;

		virtual void destroy() {}
	};
} // namespace MonsterEngine::Renderer::RHI